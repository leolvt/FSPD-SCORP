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

        streamOutputHandler sOut;
        streamInputHandler sNewWork;
        streamInputHandler sNeedMore;

        pthread_mutex_t mWorkQueue;

        std::map<int,bool> needMore;
        std::map<int,int> msgId;
        std::queue<int> workQueue;

        int handleNewWork(AHData* msg);
        int handleNeedMore(AHData* msg);
        constructFunctions(ManagerFilter);
};

#endif  /* MANAGER_FILTER_H */
