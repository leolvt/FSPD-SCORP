#ifndef MANAGER_FILTER_H
#define MANAGER_FILTER_H

#include <map>
#include <queue>
#include <fstream>
#include "eventAPI.h"

class ManagerFilter: public AHFilter
{
    public:
        ManagerFilter();
        ~ManagerFilter();

    private:
        std::ofstream log;
        int msgId;
        bool hasRequest;
        int currVal;

        streamOutputHandler sOut;
        streamInputHandler sNewWork;
        streamInputHandler sNeedMore;

        pthread_mutex_t mWorkQueue;
        pthread_mutex_t mStatus;

        std::queue<int> workQueue;

        int handleNewWork(AHData* msg);
        int handleNeedMore(AHData* msg);
        constructFunctions(ManagerFilter);
};

#endif  /* MANAGER_FILTER_H */
