#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "void.h"

typedef int Work;

int main(int argc, char *argv[]) {
    // Handle arguments
    if (argc != 3) {
        printf("Usage: %s -o <OUTFILENAME>\n", argv[0]);
        exit(0);
    }

    // Create a new work
    Work *work = (Work *) malloc( sizeof(Work) );
    *work = 26;

    // Start Anthill, append work and finalize
    char confFile[] = "./conf.xml";
    Layout *sysLayout = initDs(confFile, argc, argv);
    appendWork(sysLayout, (void*)work, sizeof(Work));
    finalizeDs(sysLayout);

    return 0;
}
