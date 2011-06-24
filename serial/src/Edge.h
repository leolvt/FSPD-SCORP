#ifndef SCORP_EDGE_H
#define SCORP_EDGE_H

#include <set>

namespace SCORP {

class Edge {
    public:
        Edge(int from, int to);
        bool operator==(const Edge& e) const;
        bool operator<(const Edge& e) const;
    private:
        int from;
        int to;
};

// =================== //

class EdgeSet {
    public:
        EdgeSet();
        EdgeSet(std::set<Edge> s);
        void addEdge(const Edge& e);
        ~EdgeSet();
    private:
        std::set<Edge> eset;
};

};

#endif  /* SCORP_EDGE_H */

