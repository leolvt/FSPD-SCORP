#ifndef SCORP_QUASI_CLIQUE_H
#define SCORP_QUASI_CLIQUE_H

#include <list>
#include "Graph.h"

namespace SCORP {

typedef struct {
    IntSet X;
    IntSet candExt;
} Candidate;

std::list<Candidate> 
process(Candidate C, double gamma, int minSize, adjHash& edges);

void
findQuasiCliques(IntSet vset, adjHash& edges, double gamma, int minSize);

};

#endif  /* SCORP_QUASI_CLIQUE_H */

