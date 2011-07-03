#ifndef SCORP_QUASI_CLIQUE_H
#define SCORP_QUASI_CLIQUE_H

#include <list>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

typedef std::tr1::unordered_set<int> IntSet;
typedef std::tr1::unordered_map< int, IntSet > adjHash;
typedef struct {
    IntSet X;
    IntSet candExt;
} Candidate;

std::list<Candidate> 
processCand(Candidate C, double gamma, int minSize, adjHash& edges, 
        Candidate& qc, bool& found);

void
findQuasiCliques(IntSet vset, adjHash& edges, double gamma, int minSize);


#endif  /* SCORP_QUASI_CLIQUE_H */

