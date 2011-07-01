#include "ManagerFilter.h"
#include <fstream>

// =================== //

ManagerFilter::ManagerFilter() 
{
    // Get Stream Handlers
    this->sOut = getOutputHandler("out");
    this->sNewWork = getInputHandler("newWork");
    this->sNeedMore = getInputHandler("needMore");

    // Open Log File
    log.open("manager.log", std::fstream::out);

    // Init Mutex
    pthread_mutex_init(&mWorkQueue, NULL);

    // Set event handlers
    setHandler( sNewWork, &ManagerFilter::handleNewWork );
    setHandler( sNeedMore, &ManagerFilter::handleNeedMore );

    // Create and keep sending work
    // TODO
    AHData* msg = new AHData(new int(1), sizeof(int), sOut);
    sendMsg(msg);
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
    int val = *((int*)msg->getData());

    log << "Got: " << val << std::endl;
    log << "Sent: " << val << std::endl;

    // Add more work to the queue
    // TODO
    if (val == 26) val = -1;
    AHData* m = new AHData(new int(val), sizeof(int), sOut);
    sendMsg(m);

    delete msg;
    return 1; 
}

// =================== //

int
ManagerFilter::handleNeedMore(AHData* msg)
{
    int val = *((int*)msg->getData());

    // Mark who needs work
    // TODO

    delete msg;
    return 1; 
}

// =================== //

provide(ManagerFilter);
