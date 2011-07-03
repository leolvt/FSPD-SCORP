#include "Graph.h"
#include <cmath>
#include <iostream>


// =================== //

Graph::Graph(IntSet vSet, adjHash& edges) {

    this->vSet = vSet;

    // Loop throug each vertex
    IntSet::iterator u;
    for(u = vSet.begin(); u != vSet.end(); u++) {
        IntSet& incVertices = edges[*u];

        // Add only those which are in vSet
        IntSet::iterator v;
        for(v = incVertices.begin(); v != incVertices.end(); v++) {
            if (vSet.find(*v) != vSet.end()) {
                this->eSet[*u].insert(*v);
            }
        }
    }

}

// =================== //

bool Graph::isQuasiClique(double gamma, int min_size) {

    // Check min_size threshold
    if (this->vSet.size() < min_size) return false;
    
    // Compute minimum connectivity number
    int cNum = (int) ceil(gamma * (this->vSet.size()-1));

    // Check the vertices
    IntSet::iterator it;
    for (it = this->vSet.begin(); it != this->vSet.end(); it++) {
        int degree = eSet[*it].size();
        if (degree < cNum) return false;
    }

    return true;
}

// =================== //

int Graph::getDegree(int vIdx) {
    return this->eSet[vIdx].size();
}

// =================== //

void Graph::removeVertex(int vIdx) {
    this->vSet.erase(vIdx);
    IntSet::iterator it;
    for (it = vSet.begin(); it != vSet.end(); it++) {
        eSet[*it].erase(vIdx);
    }
}

// =================== //

void Graph::print() {
    // Print Vertices
    IntSet::iterator u;
    std::cout << "Vertices: ";
    for(u = vSet.begin(); u != vSet.end(); u++) {
        std::cout << *u << " ";
    }
    std::cout << std::endl;

    // Print Edges
    adjHash::iterator e;
    for (e = eSet.begin(); e != eSet.end(); e++) {
        IntSet::iterator v;
        for (v = e->second.begin(); v != e->second.end(); v++) {
            std::cout << e->first << " " << *v << std::endl;
        }
    }
}

// =================== //

};
