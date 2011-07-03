#ifndef PRINTER_FILTER_H
#define PRINTER_FILTER_H

#include <list>
#include <fstream>
#include "eventAPI.h"

// =================== //

class PrinterFilter: public AHFilter
{
    public:
        PrinterFilter();
        ~PrinterFilter();
    private:
        streamInputHandler sIn;
        std::ofstream outFile;
        pthread_mutex_t mQCliques;
        
        std::list<IntSet> qCliques;
        
        int handleNewQuasiClique(AHData* msg);

        constructFunctions(PrinterFilter);

};

// =================== //

#endif  /* PRINTER_FILTER_H */

