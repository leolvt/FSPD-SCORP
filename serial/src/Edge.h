#ifndef SCORP_EDGE_H
#define SCORP_EDGE_H

#include <set>

namespace SCORP {

class Edge {
    public:
        Edge(int from, int to);
        bool operator==(const Edge& e) const;
        bool operator<(const Edge& e) const;
        int from;
        int to;
};

// =================== //

class EdgeSet {
    public:
        typedef std::set<Edge>::iterator iterator;

        EdgeSet();
        EdgeSet(std::set<Edge> s);
        ~EdgeSet();
        void addEdge(const Edge& e);
        void removeEdge(const Edge& e);
        bool hasEdge(const Edge& e);
        int size();
        iterator begin();
        iterator end();
    private:
        std::set<Edge> eset;
};

};

#endif  /* SCORP_EDGE_H */

