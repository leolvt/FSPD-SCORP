#include <fstream>
#include <iostream>
#include "Util.h"

#define SWAP(a,b) {int tmp = a; a = b; b = tmp;}

namespace SCORP {

bool parseInput(std::string filename, vHash& vertices, adjHash& edges,
        IntSet& vset) {

    // Open file
    std::cout << "File: " << filename.c_str() << std::endl;
    std::ifstream iFile(filename.c_str(), std::ifstream::in);
    int numVertices = 0;
    int numEdges;
    int numAttrib;

    if (!iFile.is_open()) {
        return false;
    }

    // Read number of Vertices
    iFile >> numVertices;

    // Read Vertices
    std::cout << "Lendo Vertices (" << numVertices << ")" << std::endl;
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
    std::cout << "Lendo Arestas ("<< numEdges <<")" << std::endl;
    for (int i = 0; i < numEdges; i++) {
        int from;
        int to;
        iFile >> from;
        iFile >> to;
        edges[from].insert(to);
        SWAP(from,to);
        edges[from].insert(to);
    }

    return true;
}

};
