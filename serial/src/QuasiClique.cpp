#include "QuasiClique.h"
#include <queue>
#include <iostream>

namespace SCORP {

// =================== //

std::list<Candidate> 
process(Candidate C, double gamma, int minSize, adjHash& edges) {
    std::list<Candidate> newCands;
    // Poda candExt

    // Create union of X and candExt
    IntSet Union = C.X;
    Union.insert(C.candExt.begin(), C.candExt.end());

    // Check if Union is quasiclique
    if (Graph(Union, edges).isQuasiClique(gamma, minSize)) {
        // As it is a quaisclique, we don't need to process the rest of the 
        // tree.

        IntSet::iterator it;
        for (it = Union.begin(); it != Union.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    else {
        // Checa X
        if ( Graph(C.X, edges).isQuasiClique(gamma, minSize) ) {
            IntSet::iterator it;
            for (it = C.X.begin(); it != C.X.end(); it++) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }

        // Extend X
        IntSet::iterator it;
        IntSet::iterator curr;
        it = C.candExt.begin();
        while (it != C.candExt.end()) {
            // Save current iterator
            curr = it++;
            
            Candidate nc;

            // Copy X + {curr}
            nc.X = C.X;
            nc.X.insert(*curr);

            // remove curr from candExt and copy it
            C.candExt.erase(curr);
            nc.candExt = C.candExt;

            // Add to list
            newCands.push_back(nc);
        }
    }

    return newCands;
}

// =================== //

void
findQuasiCliques(IntSet vset, adjHash& edges, double gamma, int minSize) {
    Candidate start;
    start.candExt = vset;
    std::queue<Candidate> qcCands;
    qcCands.push(start);
    while (!qcCands.empty()) {
        // Take a candidate
        Candidate C = qcCands.front();
        qcCands.pop();
        // Process it
        std::list<Candidate> newCands = process(C, gamma, minSize, edges);
        // Push the new ones
        std::list<Candidate>::iterator it;
        for (it = newCands.begin(); it != newCands.end(); it++) {
            qcCands.push(*it);
        }
    }
}


// =================== //

};
