#include <iostream>
#include "QuasiClique.h"
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

    cout << "Starting process" << endl;
    findQuasiCliques(vSet, Edges, 0.5, 3);

    return 0;
}
