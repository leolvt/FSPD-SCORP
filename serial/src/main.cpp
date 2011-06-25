#include <iostream>
#include "Graph.h"
#include "Util.h"
using namespace std;
using namespace SCORP;

int main(int argc, const char *argv[]) {

    if (argc < 2) return 1;

    cout << "Parsing Input" << endl;
    vHash Vertices;
    adjHash Edges;
    IntSet vSet;
    parseInput(argv[1], Vertices, Edges, vSet);
    cout << Vertices.size() << " " << Edges.size() << " " << 
        vSet.size() << endl;

    cout << "Building graph" << endl;
    Graph G(vSet, Edges);
    G.print();
    cout << "Is quasi-clique (0.5,1):  " << G.isQuasiClique(0.5, 1) << endl;

    return 0;
}
