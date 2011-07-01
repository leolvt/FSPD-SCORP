#include "ProcessFilter.h"

// =================== //

ProcessFilter::ProcessFilter() 
{
    // Init structures
    this->sIn = getInputHandler("in");
    this->sNewWork = getOutputHandler("newWork");
    this->sNeedMore = getOutputHandler("needMore");
    pthread_mutex_init(&mWorkQueue, NULL);
    pthread_mutex_init(&mWorkStatus, NULL);
    pthread_mutex_init(&mLog, NULL);
    this->msgId = 1;

    // Open Log
    std::string filename = "process.";
    filename += '0' + getMyRank();
    filename += ".log";
    log.open(filename.c_str(), std::fstream::out);
    this->stopWorking = false;

    setCPUThreads(2);

    // Set Handlers
    setHandler( sIn, &ProcessFilter::handleNewWork );

    // Create thread to process data
    pthread_create(&procThread, NULL, &ProcessFilter::process, this);
    
    pthread_mutex_lock(&mLog);
    log << "Create Thread!" << std::endl;
    pthread_mutex_unlock(&mLog);
}

// =================== //

ProcessFilter::~ProcessFilter()
{
    // Join thread
    void* res;
    pthread_join(procThread, &res);

    // Close log
    log.close();
}

// =================== //

/**
 * Check if can stop working or not
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
 * Checks if has more work to do or not.
 * Use this to check the queue size as
 * this respects the mutex.
 */
bool 
ProcessFilter::hasMoreWork()
{
    pthread_mutex_lock(&mLog);
    log << "Has More Work?";
    bool value;
    pthread_mutex_lock(&mWorkQueue);
    value = !workQueue.empty();
    pthread_mutex_unlock(&mWorkQueue);
    log << value << std::endl;
    pthread_mutex_unlock(&mLog);
    return value;
}

// =================== //

/**
 * Keep processing the local work and asking for more work,
 * until stop work signal is received.
 */
void*
ProcessFilter::process(void* param)
{
    ProcessFilter* pf = static_cast<ProcessFilter*>(param);
    while(!pf->canStop())
    {
        pthread_mutex_lock(&pf->mLog);
        pf->log << "Loop up!" << std::endl;
        pthread_mutex_unlock(&pf->mLog);

        while (pf->hasMoreWork())
        {
            pthread_mutex_lock(&pf->mLog);
            pf->log << "Inside!" << std::endl;
            pthread_mutex_unlock(&pf->mLog);

            // Get work from queue
            pthread_mutex_lock(&pf->mWorkQueue);
            int val = pf->workQueue.front();
            pf->workQueue.pop();
            pthread_mutex_unlock(&pf->mWorkQueue);

            // Do Some Work
            val++;
            AHData* data = new AHData(new int(val), sizeof(int), pf->sNewWork);
            pf->sendMsg(data);

            pthread_mutex_lock(&pf->mLog);
            pf->log << "Produced and sent work: " << val << std::endl;
            pthread_mutex_unlock(&pf->mLog);
        }   

        // Send need more msg 
        pthread_mutex_lock(&pf->mLog);
        pf->log << "Asking for more work." << std::endl;
        pthread_mutex_unlock(&pf->mLog);
        int* needMoreData = new int [2];
        needMoreData[0] = pf->getMyRank();
        needMoreData[1] = pf->msgId;
        AHData* data = new AHData(needMoreData, sizeof(int)*2, pf->sNeedMore);
        pf->sendMsg(data);
        sleep(5);
        pthread_mutex_lock(&pf->mLog);
        pf->log << "Wake up!" << std::endl;
        pthread_mutex_unlock(&pf->mLog);
    }
    
    pf->closeEventList(pf->sIn);
    return 0;
}

// =================== //

/**
 * Receives more work from manager and puts it on local queue
 */
int
ProcessFilter::handleNewWork(AHData* msg)
{
    // Get Data
    int val = *((int*)msg->getData());
    pthread_mutex_lock(&mLog);
    log << "Received: " << val << std::endl;
    pthread_mutex_unlock(&mLog);
   
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

provide(ProcessFilter);
