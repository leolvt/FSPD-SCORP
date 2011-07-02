#ifndef PROCESS_FILTER_H
#define PROCESS_FILTER_H

#include <queue>
#include <fstream>
#include "eventAPI.h"

class ProcessFilter: public AHFilter
{
    public:
        ProcessFilter();
        ~ProcessFilter();

    private:
        std::ofstream log;
        std::queue<int> workQueue;
        bool stopWorking;
        int msgId;
        int lastRequestId;

        pthread_t procThread;
        pthread_mutex_t mWorkQueue;
        pthread_mutex_t mWorkStatus;
        pthread_mutex_t mLog;

        streamInputHandler sIn;
        streamInputHandler sWorkRequest;
        streamOutputHandler sNewWork;
        streamOutputHandler sNeedMore;

        static void* process(void* param);
        bool hasMoreWork();
        bool canStop();

        int handleNewWork(AHData* msg);
        int handleWorkRequest(AHData* msg);
        constructFunctions(ProcessFilter);
};

#endif  /* PROCESS_FILTER_H */
