#ifndef SCORP_GRAPH_H
#define SCORP_GRAPH_H

#include "Vertex.h"
#include "Edge.h"

namespace SCORP {

class Graph {
    public:
        Graph(VertexSet& vs, EdgeSet& es);
        ~Graph();
        void print();
    private:
        VertexSet vs;
        EdgeSet es;
};

};

#endif  /* SCORP_GRAPH_H */

