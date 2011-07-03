#ifndef PROCESS_FILTER_H
#define PROCESS_FILTER_H

/* TODO: 
 * - Move IntSet from Util.h
 */
#include <queue>
#include <fstream>
#include "QuasiClique.h"
#include "eventAPI.h"

class ProcessFilter: public AHFilter
{
    public:
        ProcessFilter();
        ~ProcessFilter();

    private:
        std::ofstream log;
        std::queue<Candidate> workQueue;
        bool stopWorking;
        bool waitingForMore;
        int msgId;
        int lastRequestId;

        double gamma;
        int minQCSize;
        adjHash edges;

        pthread_t procThread;
        pthread_mutex_t mWorkQueue;
        pthread_mutex_t mStatus;
        pthread_mutex_t mLog;

        streamInputHandler sIn;
        streamInputHandler sWorkRequest;
        streamOutputHandler sNewWork;
        streamOutputHandler sNeedMore;
        streamOutputHandler sQCOut;

        static void* process(void* param);
        bool hasMoreWork();
        bool canStop();

        int handleNewWork(AHData* msg);
        int handleWorkRequest(AHData* msg);
        constructFunctions(ProcessFilter);
};

#endif  /* PROCESS_FILTER_H */
