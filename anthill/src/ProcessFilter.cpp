#include <list>

#include "ProcessFilter.h"
#include "Util.h"

// =================== //

/**
 * Start the Filter internal structures, open the log file
 * and start the thread responsible for actually doing some work
 */
ProcessFilter::ProcessFilter() 
{
    // Init structures: streamHandlers, mutexes, etc.
    this->sIn = getInputHandler("in");
    this->sWorkRequest = getInputHandler("workRequest");
    this->sNewWork = getOutputHandler("newWork");
    this->sNeedMore = getOutputHandler("needMore");
    this->sQCOut = getOutputHandler("qcOut");
    pthread_mutex_init(&mWorkQueue, NULL);
    pthread_mutex_init(&mStatus, NULL);
    pthread_mutex_init(&mLog, NULL);
    this->msgId = 1;
    this->lastRequestId = 0;
    this->waitingForMore = false;
    this->stopWorking = false;

    // Open Log File
    std::string filename = "process.";
    filename += '0' + getMyRank();
    filename += ".log";
    log.open(filename.c_str(), std::fstream::out);

    // Build Initial Structures
    int numVertices;
    string graphFile = getArgument("n");
    parseInput(graphFile, edges, numVertices);
    this->gamma = atof(getArgument("g").c_str());
    this->minQCSize = atoi(getArgument("q").c_str());

    // Set handler for incomming work
    setHandler( sIn, &ProcessFilter::handleNewWork );
    setHandler( sWorkRequest, &ProcessFilter::handleWorkRequest );

    // Create thread to process data
    pthread_create(&procThread, NULL, &ProcessFilter::process, this);
    pthread_mutex_lock(&mLog);
//    log << "Create Process thread!" << std::endl;
    pthread_mutex_unlock(&mLog);
}

// =================== //

/**
 * Filter Destructor. Wait for processing thread to finish and 
 * close the log file.
 */
ProcessFilter::~ProcessFilter()
{
    // Join thread
    void* res;
    pthread_join(procThread, &res);

    // Close log
    pthread_mutex_lock(&mLog);
    log.close();
    pthread_mutex_unlock(&mLog);
}

// =================== //

/**
 * Check if the filter can stop working or not
 * Use this to check the stopWorking variable as
 * this respects the mutex.
 */
bool 
ProcessFilter::canStop()
{
    bool value;
    pthread_mutex_lock(&mStatus);
    value = stopWorking;
    pthread_mutex_unlock(&mStatus);
    return value;
}

// =================== //

/**
 * Checks if the filter has more work to do or not.
 * Use this to check the queue size as
 * this respects the mutex.
 */
bool 
ProcessFilter::hasMoreWork()
{
    bool value;
    pthread_mutex_lock(&mWorkQueue);
    value = !workQueue.empty();
    pthread_mutex_unlock(&mWorkQueue);
    return value;
}

// =================== //

/**
 * This is the main processing function. Interaction with 
 * other filters are handled throught the stream handlers.
 * Keep processing the local work and asking for more work,
 * until stop work signal is received.
 */
void*
ProcessFilter::process(void* param)
{
    /* 
     * As this is a static method call by 'pthread_create', the
     * actual filter instance is passed through the param.
     */
    ProcessFilter* pf = static_cast<ProcessFilter*>(param);

    // We only stop when the Manager tells us to.
    while(!pf->canStop())
    {

        // While we have local work, we must process it.
        while (pf->hasMoreWork())
        {

            // Get work from queue
            pthread_mutex_lock(&pf->mWorkQueue);
            Candidate candidate = pf->workQueue.front();
            pf->workQueue.pop();
            int qSize = pf->workQueue.size();
            pthread_mutex_unlock(&pf->mWorkQueue);

            // Do Some Work
            Candidate qc;
            bool found;
            std::list<Candidate> newCands = processCand(candidate, pf->gamma, 
                                                    pf->minQCSize, pf->edges,
                                                    qc, found);
            // Send QuasiClique if it was found
            if (found)
            {
                std::list<Candidate> qcList;
                qcList.push_back(qc);
                size_t msgSize;
                int* qciMsg = list2Msg(qcList, pf->getMyRank(), msgSize);
                AHData* qcMsg = new AHData(qciMsg, msgSize, pf->sQCOut);
                pf->sendMsg(qcMsg);
            }

            // Send half of the work done back to manager
            // and store the other half 
            pthread_mutex_lock(&pf->mWorkQueue);
            int halfWorkAmount = newCands.size() / 2;
            int totalWorkSent = 0;
            while (totalWorkSent < halfWorkAmount)
            {
                Candidate& candidate = newCands.front();
                pf->workQueue.push(candidate);
                newCands.pop_front();
                totalWorkSent++;
            }
            pthread_mutex_unlock(&pf->mWorkQueue);
            
            // Create and send msg
            if (!newCands.empty()) {
                size_t msgSize;
                int* pMsg = list2Msg(newCands, 0, msgSize);
                AHData* data = new AHData(pMsg, msgSize, pf->sNewWork);
                pf->sendMsg(data);
            }
            
            // Log work done
            pthread_mutex_lock(&pf->mLog);
            pf->log << "Processed new candidate" << std::endl;
            pthread_mutex_unlock(&pf->mLog);
        }   

        // Send  the "need more work" msg, when we run out of work
        pthread_mutex_lock(&pf->mLog);
        pf->log << "Asking for more work." << std::endl;
        pthread_mutex_unlock(&pf->mLog);
        pthread_mutex_lock(&pf->mStatus);
        int* needMoreData = new int [2];
        needMoreData[0] = pf->getMyRank();
        needMoreData[1] = pf->msgId;
        pf->waitingForMore = true;
        AHData* data = new AHData(needMoreData, sizeof(int)*2, pf->sNeedMore);
        pf->sendMsg(data);
        pthread_mutex_unlock(&pf->mStatus);

        // I needed a sleep here to actually make it work, as otherwise no
        // processing was done to receive the message with more work
        usleep(500);
        
    }
    
    /*
     * When we can safely stop working, close the event lists, 
     * breaking the message loop.
     */
    pthread_mutex_lock(&pf->mLog);
    pf->log << "Closing event lists" << std::endl;
    pthread_mutex_unlock(&pf->mLog);
    pf->closeEventList(pf->sIn);
    pf->closeEventList(pf->sWorkRequest);

    return 0;
}

// =================== //

/**
 * Receives more work from manager and puts it on local queue
 */
int
ProcessFilter::handleNewWork(AHData* msg)
{
    // Translate the data to the work
    int* pMsg = (int *) msg->getData();
    int id = msg2Id(pMsg);
    if (id != getMyRank() && id != EOW_ID) return 0;
  
    // Check for message type
    if (id == EOW_ID) 
    {
        pthread_mutex_lock(&mLog);
//        log << "Received End of Work." << std::endl;
        pthread_mutex_unlock(&mLog);
        
        pthread_mutex_lock(&mStatus);
        stopWorking = true;
        pthread_mutex_unlock(&mStatus);
    }
    else
    {
        // Log it
        pthread_mutex_lock(&mLog);
//        log << "Received new message" << std::endl;
        pthread_mutex_unlock(&mLog);

        // Read and put it on the queue
        std::cout << "<< INSIDE handleNewWork >>" << std::endl;
        std::cout << "<< MSG: "<< pMsg <<" >>" << std::endl;
        std::cout << "<< RANK: "<< getMyRank() <<" >>" << std::endl;
        std::list<Candidate> newSets = msg2List(pMsg);

        pthread_mutex_lock(&mWorkQueue);
        std::list<Candidate>::iterator it;
        for (it = newSets.begin(); it != newSets.end(); it++)
        {
            workQueue.push(*it);
        }
        pthread_mutex_unlock(&mWorkQueue);

        // Update Msg ID
        pthread_mutex_lock(&mStatus);
        if (waitingForMore) {
            waitingForMore = false;
            msgId++;
        }
        pthread_mutex_unlock(&mStatus);
    }

    return 0; 
}

// =================== //

/**
 * Handle a work request from the manager.
 * If we have local work, send half of it to the manager.
 */
int
ProcessFilter::handleWorkRequest(AHData* msg)
{
    int requestId = *((int*)msg->getData());

    // Log request
    pthread_mutex_lock(&mLog);
//    log << "Received Work Request from manager." << std::endl;
    pthread_mutex_unlock(&mLog);

    // Check message ID
    // We only answer to expected message
    pthread_mutex_lock(&mStatus);
    if (requestId <= this->lastRequestId) 
    {   
        pthread_mutex_unlock(&mStatus);
        return 0;
    }
    pthread_mutex_unlock(&mStatus);

    // Lock work queue and send half of current work to the manager
    pthread_mutex_lock(&mWorkQueue);
    int halfWorkAmount = workQueue.size() / 2;
    int totalWorkSent = 0;
    std::list<Candidate> worksToSend;
    while (totalWorkSent < halfWorkAmount)
    {
        // Gather IntSets
        Candidate& candidate = workQueue.front();
        worksToSend.push_back(candidate);
        workQueue.pop();
        totalWorkSent++;
    }
    pthread_mutex_lock(&mStatus);
    this->lastRequestId++;    
    pthread_mutex_unlock(&mStatus);
    pthread_mutex_unlock(&mWorkQueue);

    // Create and send msg
    if (!worksToSend.empty()) {
        size_t msgSize;
        int* pMsg = list2Msg(worksToSend, 0, msgSize);
        AHData* data = new AHData(pMsg, msgSize, sNewWork);
        sendMsg(data);
    }

    return 0; 
}

// =================== //

provide(ProcessFilter);
