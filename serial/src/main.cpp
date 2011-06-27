#include <iostream>
#include "QuasiClique.h"
#include "Graph.h"
#include "Util.h"
using namespace std;
using namespace SCORP;

int main(int argc, const char *argv[]) {

    if (argc < 2) return 1;

    cout << "Parsing Input" << endl;
    adjHash Edges;
    IntSet vSet;
    if (!parseInput(argv[1], Edges, vSet)) {
        cout << "Erro ao ler entrada!" << endl;
        return 1;
    }

    cout << "Starting process" << endl;
    cout.flush();
    findQuasiCliques(vSet, Edges, 0.7, 15);

    return 0;
}
