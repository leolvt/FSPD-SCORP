#include "Graph.h"
#include <iostream>

namespace SCORP {

// =================== //

Graph::Graph(VertexSet& vs, EdgeSet& es) {
    this->vs = vs;
    this->es = es;
}

// =================== //

Graph::~Graph() {
    
}

// =================== //

void Graph::print() {

    // Print vertices
    std::cout << vs.size() << std::endl;
    VertexSet::iterator it;
    for (it = vs.begin(); it != vs.end(); it++) {
        std::cout << *it << std::endl;
    }

    // Print Edges
    std::cout << es.size() << std::endl;
    EdgeSet::iterator eIt;
    for(eIt = es.begin(); eIt != es.end(); eIt++) {
        std::cout << eIt->from << " " << eIt->to << std::endl;
    }
}

// =================== //

};
