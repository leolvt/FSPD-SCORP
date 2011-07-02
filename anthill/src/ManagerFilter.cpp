#include "ManagerFilter.h"
#include <fstream>

// =================== //

ManagerFilter::ManagerFilter() 
{
    // Initialize structures: streams, vars, mutexes, etc.
    this->sOut = getOutputHandler("out");
    this->sNewWork = getInputHandler("newWork");
    this->sNeedMore = getInputHandler("needMore");
    this->currVal = 0;
    this->msgId = 1;
    this->hasRequest = false;
    pthread_mutex_init(&mWorkQueue, NULL);
    pthread_mutex_init(&mStatus, NULL);

    // Open Log File
    log.open("manager.log", std::fstream::out);

    // Set event handlers
    setHandler( sNewWork, &ManagerFilter::handleNewWork );
    setHandler( sNeedMore, &ManagerFilter::handleNeedMore );

    // Create initial work 
    // TODO
}

// =================== //

ManagerFilter::~ManagerFilter()
{
    // Close Log File
    log.close();
}

// =================== //

int
ManagerFilter::handleNewWork(AHData* msg)
{
    // TODO: Change work handling and logging
    // (perhaps use mutex for the lgo.)
    int val = *((int*)msg->getData());

    // Handle response to request: increase msgId
    pthread_mutex_lock(&mStatus);
    if (this->hasRequest) 
    {
        this->msgId++;
        this->hasRequest = false;
    }
    pthread_mutex_unlock(&mStatus);

    // Add work received to queue
    // TODO

    delete msg;
    return 1; 
}

// =================== //

int
ManagerFilter::handleNeedMore(AHData* msg)
{
    int val = *((int*)msg->getData());

    // Handle Work Request
    // TODO: Converto to int*: (id, msgId)
    // Check if we should answer the request.
    if (this->currVal++ == 6) this->currVal = -1;
    AHData* m = new AHData(new int(this->currVal), sizeof(int), sOut);
    sendMsg(m);
    
    // TODO
    // If we have work, send to client and mark that he has work
    // If we don't: 
    //      mark that he doesn't have work
    //      Loop through all known clients.
    //      If none has work to do, we can stop
    //      Send EOF message!

    delete msg;
    return 1; 
}

// =================== //

provide(ManagerFilter);
