#include <string>
#include "Util.h"
#include "QuasiClique.h"
#include "PrinterFilter.h"

// =================== //

PrinterFilter::PrinterFilter()
{
    std::string filename = getArgument("o");
    this->sIn = getInputHandler("in");
    this->outFile.open(filename.c_str(), std::fstream::out);

    pthread_mutex_init(&mQCliques, NULL);

    setHandler( sIn, &PrinterFilter::handleNewQuasiClique );
}

// =================== //

PrinterFilter::~PrinterFilter()
{
    std::list<IntSet>::iterator lIt;
    for (lIt = qCliques.begin(); lIt != qCliques.end(); lIt++)
    {
        IntSet::iterator it;
        for (it = lIt->begin(); it != lIt->end(); it++)
        {
            outFile << *it << " ";
        }
        outFile << std::endl;
    }
    this->outFile.close();
}

// =================== //

int
PrinterFilter::handleNewQuasiClique(AHData * msg)
{
    pthread_mutex_lock(&mQCliques);
    Candidate newQCList = msg2List((int*) msg->getData()).front();


    std::cout << "====== QUASICLIQUE: =======" << std::endl;
    IntSet::iterator it;
    for (it = newQCList.X.begin(); it != newQCList.X.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    std::list<IntSet>::iterator lIt = qCliques.begin();
    std::list<IntSet>::iterator aux;
    bool isNew = true;
    while (lIt != qCliques.end())
    {
        aux = lIt++;
        if ( isSuperSet(*aux, newQCList.X) )
        {
            isNew = false;
            break;
        }
        else
        {
            if ( isSuperSet(newQCList.X, *aux) ) {
                qCliques.erase(aux);
            }
        }
    }
    if (isNew) 
    {
        qCliques.push_back(newQCList.X);
    }
    pthread_mutex_unlock(&mQCliques);

    delete msg;
    return 0;
}

// =================== //

provide(PrinterFilter);
