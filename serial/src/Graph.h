#ifndef SCORP_GRAPH_H
#define SCORP_GRAPH_H

#include <set>
#include <tr1/unordered_map>
#include "Attribute.h"

namespace SCORP {

typedef std::set<int> IntSet;
typedef std::set<Attribute> AttribSet;

typedef std::tr1::unordered_map< int, AttribSet > vHash;
typedef std::tr1::unordered_map< int, IntSet > adjHash;

class Graph {
    public:
        Graph(IntSet vSet, adjHash& Edges);
        bool isQuasiClique(double gamma, int minSize);
        void print();
    private:
        IntSet vSet;
        adjHash eSet;
};

};

#endif  /* SCORP_GRAPH_H */

