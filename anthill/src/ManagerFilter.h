#ifndef MANAGER_FILTER_H
#define MANAGER_FILTER_H

/* TODO: 
 * - Move IntSet from Util.h
 * - Define EOW_ID elsewhere
 */
#include <map>
#include <queue>
#include <fstream>
#include "eventAPI.h"
#include "Util.h"


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

        double gamma;
        int minQCSize;
        adjHash edges;

        streamOutputHandler sOut;
        streamOutputHandler sWorkRequest;
        streamInputHandler sNewWork;
        streamInputHandler sNeedMore;

        pthread_mutex_t mWorkQueue;
        pthread_mutex_t mStatus;
        pthread_mutex_t mLog;

        std::map<int,int> lastId;
        std::map<int,bool> hasWork;
        std::queue<Candidate> workQueue;

        void buildInitialWork();
        int handleNewWork(AHData* msg);
        int handleNeedMore(AHData* msg);
        constructFunctions(ManagerFilter);
};

#endif  /* MANAGER_FILTER_H */
