#include <fstream>

#include "Util.h"
#include "ManagerFilter.h"

// =================== //

ManagerFilter::ManagerFilter() 
{
    // Initialize structures: streams, vars, mutexes, etc.
    this->sOut = getOutputHandler("out");
    this->sWorkRequest = getOutputHandler("workRequest");
    this->sNewWork = getInputHandler("newWork");
    this->sNeedMore = getInputHandler("needMore");
    this->currVal = 0;
    this->msgId = 1;
    this->hasRequest = false;
    pthread_mutex_init(&mWorkQueue, NULL);
    pthread_mutex_init(&mStatus, NULL);
    pthread_mutex_init(&mLog, NULL);

    // Open Log File
    log.open("manager.log", std::fstream::out);

    // Create initial work 
    // TODO
    cand_t C;
    C.X.push_back(1);
    C.candExt.push_back(2);
    C.candExt.push_back(3);
    workQueue.push(C);
    
    // Set event handlers
    setHandler( sNewWork, &ManagerFilter::handleNewWork );
    setHandler( sNeedMore, &ManagerFilter::handleNeedMore );
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
    // Convert Msg
    int* cMsg = (int*) msg->getData();
    std::list<cand_t> newWork = msg2List(cMsg);
    pthread_mutex_lock(&mLog);
    log << "Received work, "  << newWork.size() << " more candidates." << std::endl;
    pthread_mutex_unlock(&mLog);


    // Handle response to request: increase msgId
    pthread_mutex_lock(&mStatus);
    if (this->hasRequest) 
    {
        this->msgId++;
        this->hasRequest = false;
    }
    pthread_mutex_unlock(&mStatus);

    // Add work received to queue
    pthread_mutex_lock(&mWorkQueue);
    std::list<cand_t>::iterator it;
    for (it = newWork.begin(); it != newWork.end(); it++)
    {
        this->workQueue.push(*it);
    }
    pthread_mutex_unlock(&mWorkQueue);

    delete msg;
    return 1; 
}

// =================== //

int
ManagerFilter::handleNeedMore(AHData* msg)
{
    int* iMsg = (int*)msg->getData();
    int pId = iMsg[0];
    int mId = iMsg[1];

    pthread_mutex_lock(&mLog);
    log << "Received work request from " << pId << " with mId: " << mId;
    log << ", expected mId: " << lastId[pId]+1 << std::endl;
    pthread_mutex_unlock(&mLog);

    // Handle Work Request
    // Check if we should answer the request.
    if (mId <= lastId[pId] ) return 1;

    int* pMsg;
    size_t msgSize = 0;
    pthread_mutex_lock(&mWorkQueue);
    // If we have work
    if (!workQueue.empty()) 
    {
        // Mark that he has work and update lastId
        hasWork[pId] = true;
        lastId[pId]++;

        // Build char message
        std::list<cand_t> workToSend;
        workToSend.push_front(workQueue.front());
        workQueue.pop();
        pMsg = list2Msg(workToSend, pId, msgSize);
    }
    // If we don't have work
    else
    {
        // Mark no work for this pId
        hasWork[pId] = false;

        // Loop through all known clients, to see if someone has work
        bool someWork = false;
        std::map<int,bool>::iterator it;
        for (it = hasWork.begin(); it != hasWork.end(); it++) 
        {
            if (it->second)
            {
                someWork = true;
                break;
            }
        }

        // If none has work to do, we can stop
        if (!someWork) 
        {
            pMsg = buildEowMsg(msgSize);
            closeEventList(this->sNeedMore);
            closeEventList(this->sNewWork);
        }
        // Otherwise, ask for work
        else
        {
            // Send msg
            pthread_mutex_lock(&mStatus);
            if (!this->hasRequest) {
                this->hasRequest = true;
                AHData* askForMore = new AHData(new int(msgId), sizeof(int), 
                                                sWorkRequest);
                sendMsg(askForMore);
            }
            pthread_mutex_unlock(&mStatus);
            pMsg = 0;
        }
    }
    pthread_mutex_unlock(&mWorkQueue);

    if (msgSize != 0)
    {
        std::cout << ">>>>> MANAGER <<<<<<" << std::endl;
        std::cout << " >> MSG: " << pMsg << std::endl;
        AHData* d = new AHData(pMsg, msgSize, sOut);
        sendMsg(d);
    }

    delete msg;
    return 1; 
}

// =================== //

provide(ManagerFilter);
