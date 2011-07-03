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

    pthread_mutex_init(&mOutFile, NULL);

    setHandler( sIn, &PrinterFilter::handleNewQuasiClique );
}

// =================== //

PrinterFilter::~PrinterFilter()
{
    this->outFile.close();
}

// =================== //

int
PrinterFilter::handleNewQuasiClique(AHData * msg)
{
    pthread_mutex_lock(&mOutFile);
    Candidate newQCList = msg2List((int*) msg->getData()).front();
    IntSet::iterator it;
    for (it = newQCList.X.begin(); it != newQCList.X.end(); it++)
    {
        outFile << *it << " ";
    }
    outFile << std::endl;
    pthread_mutex_unlock(&mOutFile);

    delete msg;
    return 0;
}

// =================== //

provide(PrinterFilter);
