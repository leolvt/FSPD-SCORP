#include "QuasiClique.h"
#include <cmath>
#include <queue>
#include <stack>
#include <iostream>


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

bool 
isQC(IntSet vSet, adjHash& edges, double gamma, int minSize) {
    if (vSet.size() < minSize) return false;
    int threshold = (int) ceil(gamma*(vSet.size()-1));
    IntSet::iterator u;
    IntSet::iterator v;
    for (u = vSet.begin(); u != vSet.end(); u++) {
        int degree = 0;
        for (v = edges[*u].begin(); v != edges[*u].end(); v++) {
            if (vSet.find(*v) != vSet.end()) {
                degree++;
            }
        }
        if (degree < minSize) return false;
    }
    return true;
}

// =================== //

std::list<Candidate> 
processCand(Candidate C, double gamma, int minSize, adjHash& edges, 
        Candidate& qc, bool& found)
{
    found = false; 
    std::list<Candidate> newCands;
    IntSet::iterator it;

    std::cout << "====== X SET: =======" << std::endl;
    for (it = C.X.begin(); it != C.X.end(); it++)
    {
        std::cout << *it << " " << std::endl;
    }
    std::cout << std::endl;
    std::cout << "====== candExt SET: =======" << std::endl;
    for (it = C.candExt.begin(); it != C.candExt.end(); it++)
    {
        std::cout << *it << " " << std::endl;
    }
    std::cout << std::endl;

    // Diameter (Reachability) prunning 
    if (C.X.size() > 0) 
    {
        IntSet reachable = getReachable(C.X, edges, 2);
        IntSet::iterator it = C.candExt.begin();;
        while (it != C.candExt.end()) 
        {
            IntSet::iterator aux = it;
            it++;
            if (reachable.find(*aux) == reachable.end()) 
            {
                C.candExt.erase(aux);
            }
        }
    } 

    // Degree prunning
    int threshold = (int) ceil(gamma*(minSize-1));
    it = C.candExt.begin();;
    while (it != C.candExt.end()) 
    {
        IntSet::iterator aux = it;
        it++;

        int degree = 0;
        IntSet::iterator i;
        for (i = edges[*aux].begin(); i != edges[*aux].end(); i++) 
        {
            if (C.X.find(*i) != C.X.end() || 
                    C.candExt.find(*i) != C.candExt.end()) 
            {
                degree++;
            }
        }

        if (degree  < threshold) 
        {
            C.candExt.erase(aux);
        }
    }

    // Degree prunning # 2
    int numRemoved = 0;
    do 
    {
        numRemoved = 0;
        it = C.candExt.begin();
        while (it != C.candExt.end()) 
        {
            IntSet::iterator aux = it;
            it++;

            int inDegree = 0;
            int extDegree = 0;
            IntSet::iterator it2;
            for (it2 = edges[*aux].begin(); it2 != edges[*aux].end(); it2++) 
            {
                if (C.X.find(*it2) != C.X.end()) 
                {
                    inDegree++;
                }
                else if (C.candExt.find(*it2) != C.candExt.end()) 
                {
                    extDegree++;
                }
            }

            int uThreshold = (int) ceil(gamma*(extDegree+C.X.size()-2));

            if (inDegree + extDegree < uThreshold) 
            {
                numRemoved++;
                C.candExt.erase(aux);
            }
        }
    }
    while (numRemoved > 0); 
  
    // Check for Fail Vertices in X
    bool failVertice = false;
    for (it = C.X.begin(); it != C.X.end(); it++) 
    {
            int inDegree = 0;
            int extDegree = 0;
            IntSet::iterator it2;
            for (it2 = edges[*it].begin(); it2 != edges[*it].end(); it2++) 
            {
                if (C.X.find(*it2) != C.X.end()) 
                {
                    inDegree++;
                }
                else if (C.candExt.find(*it2) != C.candExt.end()) 
                {
                    extDegree++;
                }
            }
            int th1 = (int) ceil(gamma*C.X.size());
            int th2 = (int) ceil(gamma*(C.X.size() - 1 + extDegree));
            if ( (extDegree == 0 && inDegree < th1) || 
                   (inDegree+extDegree < th2) ) 
            {

                failVertice = true;
                break; 
            }
    }
    if (failVertice) 
    {
        C.candExt.clear();
    }

    // Check if Union is quasiclique
    IntSet Union(C.X);
    Union.insert(C.candExt.begin(), C.candExt.end());
    if (isQC(Union, edges, gamma, minSize)) 
    {
        // As it is a quaisclique, we don't need to process the rest of the 
        // tree.
        // TODO: send QuasiClique
        // Build and send Msg
        qc.X = Union;
        found = true;
    }
    else 
    {
        // Checa X
        if ( isQC(C.X, edges, gamma, minSize) ) 
        {
            qc.X = Union;
            found = true;
            /* TODO: send quasiClique
            IntSet::iterator it;
            for (it = C.X.begin(); it != C.X.end(); it++) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
            */
        }

        // Extend X
        IntSet::iterator it;
        IntSet::iterator curr;
        it = C.candExt.begin();
        while (it != C.candExt.end()) 
        {
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
    double numProcessed = 0.0;

    int min = (int) ceil(gamma*(minSize-1));
    IntSet::iterator it = start.candExt.begin();
    while (it != start.candExt.end()) {
        IntSet::iterator aux = it;
        it++;
        if (edges[*aux].size() < min) {
            start.candExt.erase(aux);
        }
    }
    std::cerr << "Num Vertices: " << start.candExt.size() << std::endl;

    std::stack<Candidate> qcCands;
    qcCands.push(start);
    while (!qcCands.empty()) {
        // Take a candidate
        Candidate C = qcCands.top();
        qcCands.pop();
        numProcessed++;
        // Process it
        Candidate qc;
        bool found;
        std::list<Candidate> newCands = processCand(C, gamma, minSize, edges, 
                                                    qc, found);
        // Push the new ones
        std::list<Candidate>::iterator it;
        for (it = newCands.begin(); it != newCands.end(); it++) {
            qcCands.push(*it);
        }
    }
}


// =================== //

