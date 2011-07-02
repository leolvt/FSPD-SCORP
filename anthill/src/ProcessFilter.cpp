#include "ProcessFilter.h"

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
    pthread_mutex_init(&mWorkQueue, NULL);
    pthread_mutex_init(&mWorkStatus, NULL);
    pthread_mutex_init(&mLog, NULL);
    this->msgId = 1;
    this->lastRequestId = 0;
    this->stopWorking = false;

    // Open Log File
    std::string filename = "process.";
    filename += '0' + getMyRank();
    filename += ".log";
    log.open(filename.c_str(), std::fstream::out);

    // Set handler for incomming work
    setHandler( sIn, &ProcessFilter::handleNewWork );
    setHandler( sWorkRequest, &ProcessFilter::handleWorkRequest );

    // Create thread to process data
    pthread_create(&procThread, NULL, &ProcessFilter::process, this);
    pthread_mutex_lock(&mLog);
    log << "Create Process thread!" << std::endl;
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
    pthread_mutex_lock(&mWorkStatus);
    value = stopWorking;
    pthread_mutex_unlock(&mWorkStatus);
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
            int val = pf->workQueue.front();
            pf->workQueue.pop();
            pthread_mutex_unlock(&pf->mWorkQueue);

            // Do Some Work
            // TODO: Call functions here
            
            // Log work done
            pthread_mutex_lock(&pf->mLog);
            pf->log << "Produced and sent work: " << val << std::endl;
            pthread_mutex_unlock(&pf->mLog);
        }   

        // Send  the "need more work" msg, when we run out of work
        pthread_mutex_lock(&pf->mLog);
        pf->log << "Asking for more work." << std::endl;
        pthread_mutex_unlock(&pf->mLog);
        int* needMoreData = new int [2];
        needMoreData[0] = pf->getMyRank();
        needMoreData[1] = pf->msgId;
        AHData* data = new AHData(needMoreData, sizeof(int)*2, pf->sNeedMore);
        pf->sendMsg(data);

        // I needed a sleep here to actually make it work, as otherwise no
        // processing was done to receive the message with more work
        sleep(1);
    }
    
    /*
     * When we can safely stop working, close the event lists, 
     * breaking the message loop.
     */
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
    int val = *((int*)msg->getData());
    pthread_mutex_lock(&mLog);
    log << "Received: " << val << std::endl;
    pthread_mutex_unlock(&mLog);
  
    // Check for message type
    if (val == -1) 
    {
        // Received stop work signal
        pthread_mutex_lock(&mWorkStatus);
        stopWorking = true;
        pthread_mutex_unlock(&mWorkStatus);
    }
    else
    {
        // Put it on the queue
        pthread_mutex_lock(&mWorkQueue);
        workQueue.push(val);
        pthread_mutex_unlock(&mWorkQueue);
        msgId++;
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

    // Translate the data to the work
    pthread_mutex_lock(&mLog);
    log << "Received Work Request from manager." << std::endl;
    pthread_mutex_unlock(&mLog);

    // Check message ID
    // We only answer to expected message
    if (requestId <= this->lastRequestId) return 0;

    // Lock work queue and send half of current work to the manager
    pthread_mutex_lock(&mWorkQueue);
    int halfWorkAmount = workQueue.size() / 2;
    int totalWorkSent = 0;
    while (totalWorkSent < halfWorkAmount)
    {
        // Send work
        // TODO
        totalWorkSent++;
    }
    this->lastRequestId++;    
    pthread_mutex_unlock(&mWorkQueue);

    return 0; 
}

// =================== //

provide(ProcessFilter);
