#include <fstream>
#include "Util.h"

namespace SCORP {

void parseInput(std::string filename, std::map<int, Vertex>& v, VertexSet& vs, 
                EdgeSet& es) {

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
        Vertex vtx(i+1);
        for (int a = 0; a < numAttrib; a++) {
            std::string aStr;
            iFile >> aStr;
            Attribute A(aStr);
            vtx.addAttribute(A);
        }
        v[i+1] = vtx;
        vs.addVertex(i+1);
    }

    // Read number of Edges
    iFile >> numEdges;

    // Read Edges
    for (int i = 0; i < numEdges; i++) {
        int from;
        int to;
        iFile >> from;
        iFile >> to;
        Edge E(from, to);
        es.addEdge(E);
    }

}

};
