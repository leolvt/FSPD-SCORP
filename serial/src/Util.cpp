#include <fstream>
#include "Util.h"

#define SWAP(a,b) {int tmp = a; a = b; b = tmp;}

namespace SCORP {

void parseInput(std::string filename, vHash& vertices, adjHash& edges,
        IntSet& vset) {

    // Open file
    std::ifstream iFile(filename.c_str(), std::ifstream::in);
    int numVertices;
    int numEdges;
    int numAttrib;

    // Read number of Vertices
    iFile >> numVertices;

    // Read Vertices
    for (int i = 0; i < numVertices; i++) {
        iFile >> numAttrib;
        AttribSet& aSet = vertices[i+1];
        for (int a = 0; a < numAttrib; a++) {
            std::string aStr;
            iFile >> aStr;
            Attribute A(aStr);
            aSet.insert(A);
        }
        vset.insert(i+1);
    }

    // Read number of Edges
    iFile >> numEdges;

    // Read Edges
    for (int i = 0; i < numEdges; i++) {
        int from;
        int to;
        iFile >> from;
        iFile >> to;
        edges[from].insert(to);
        SWAP(from,to);
        edges[from].insert(to);
    }

}

};
