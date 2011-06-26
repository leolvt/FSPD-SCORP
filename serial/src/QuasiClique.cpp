#include "QuasiClique.h"
#include <cmath>
#include <queue>
#include <stack>
#include <iostream>

namespace SCORP {

// =================== //

IntSet
getReachable(IntSet X, adjHash& edges, int k) {
    if (k == 0) {
        return IntSet(X);
    }
    IntSet reachable(X);
    for (IntSet::iterator it = X.begin(); it != X.end(); it++) {
        IntSet n = getReachable(edges[*it], edges, k-1);
        reachable.insert(n.begin(), n.end());
    }
    return reachable;
}

// =================== //

std::list<Candidate> 
process(Candidate C, double gamma, int minSize, adjHash& edges) {
   
    std::list<Candidate> newCands;
    IntSet::iterator it;

    // Create union of X and candExt
    IntSet Union(C.X);
    Union.insert(C.candExt.begin(), C.candExt.end());
    Graph uG(Union, edges);
    
    // Diameter (Reachability) prunning 
    if (C.X.size() > 0) {
        IntSet reachable = getReachable(C.X, edges, 2);
        IntSet::iterator it = C.candExt.begin();;
        while (it != C.candExt.end()) {
            IntSet::iterator aux = it;
            it++;
            if (reachable.find(*aux) == reachable.end()) {
                C.candExt.erase(aux);
                uG.removeVertex(*aux);
            }
        }
    } 

    // Degree prunning
    int threshold = (int) ceil(gamma*(minSize-1));
    it = C.candExt.begin();;
    while (it != C.candExt.end()) {
        IntSet::iterator aux = it;
        it++;
        if (uG.getDegree(*aux) < threshold) {
            C.candExt.erase(aux);
            uG.removeVertex(*aux);
        }
    }

    // Degree prunning # 2
    int numRemoved = 0;
    do {
        numRemoved = 0;
        it = C.candExt.begin();
        while (it != C.candExt.end()) {
            IntSet::iterator aux = it;
            it++;

            int inDegree = 0;
            int extDegree = 0;
            IntSet::iterator it2;
            for (it2 = edges[*aux].begin(); it2 != edges[*aux].end(); it2++) {
                if (C.X.find(*it2) != C.X.end()) {
                    inDegree++;
                }
                else if (C.candExt.find(*it2) != C.candExt.end()) {
                    extDegree++;
                }
            }

            int uThreshold = (int) ceil(gamma*(extDegree+C.X.size()-2));

            if (inDegree + extDegree < uThreshold) {
                numRemoved++;
                C.candExt.erase(aux);
                uG.removeVertex(*aux);
            }
        }
    }
    while (numRemoved > 0); 
  
    // Check for Fail Vertices in X
    bool failVertice = false;
    for (it = C.X.begin(); it != C.X.end(); it++) {
            int inDegree = 0;
            int extDegree = 0;
            IntSet::iterator it2;
            for (it2 = edges[*it].begin(); it2 != edges[*it].end(); it2++) {
                if (C.X.find(*it2) != C.X.end()) {
                    inDegree++;
                }
                else if (C.candExt.find(*it2) != C.candExt.end()) {
                    extDegree++;
                }
            }
            int th1 = (int) ceil(gamma*C.X.size());
            int th2 = (int) ceil(gamma*(C.X.size() - 1 + extDegree));
            if ( (extDegree == 0 && inDegree < th1) || 
                   (inDegree+extDegree < th2) ) {

                failVertice = true;
                break; 
            }
    }
    if (failVertice) {
        C.candExt.clear();
        uG = Graph(C.X, edges);
    }

    // Check if Union is quasiclique
    Union = C.X;
    Union.insert(C.candExt.begin(), C.candExt.end());
    uG = Graph(Union, edges);
    if (uG.isQuasiClique(gamma, minSize)) {
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

    int min = (int) ceil(gamma*(minSize-1));
    IntSet::iterator it = start.candExt.begin();
    while (it != start.candExt.end()) {
        IntSet::iterator aux = it;
        it++;
        if (edges[*aux].size() < min) {
            start.candExt.erase(aux);
        }
    }

    std::stack<Candidate> qcCands;
    qcCands.push(start);
    while (!qcCands.empty()) {
        // Take a candidate
        Candidate C = qcCands.top();
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
