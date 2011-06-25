#include <iostream>
#include <map>
#include "Vertex.h"
#include "Edge.h"
#include "Graph.h"
#include "Util.h"
using namespace std;
using namespace SCORP;

int main(int argc, const char *argv[]) {

    if (argc < 2) return 1;

    map<int, Vertex> vMap;
    VertexSet vs;
    EdgeSet es;
    
    cout << "Parsing Input" << endl;

    parseInput(argv[1], vMap, vs, es);
    cout << vMap.size() << " " << vs.size() << " " << es.size() << endl;

    cout << "Building graph" << endl;
    Graph G(vs, es);


    G.print();

    return 0;
}
