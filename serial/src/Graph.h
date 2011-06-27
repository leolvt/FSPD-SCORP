#ifndef SCORP_GRAPH_H
#define SCORP_GRAPH_H

#include <set>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace SCORP {

typedef std::tr1::unordered_set<int> IntSet;

//typedef std::tr1::unordered_map< int, AttribSet > vHash;
typedef std::tr1::unordered_map< int, IntSet > adjHash;

class Graph {
    public:
        Graph(IntSet vSet, adjHash& Edges);
        bool isQuasiClique(double gamma, int minSize);
        int getDegree(int vIdx);
        void removeVertex(int vIdx);
        void print();
    private:
        IntSet vSet;
        adjHash eSet;
};

};

#endif  /* SCORP_GRAPH_H */

