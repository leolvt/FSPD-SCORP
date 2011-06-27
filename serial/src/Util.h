#ifndef SCORP_UTIL_H
#define SCORP_UTIL_H

#include <string>
#include <map>
#include "Graph.h"

namespace SCORP {

bool parseInput(std::string filename, adjHash& edges, IntSet& vset);

};

#endif  /* SCORP_UTIL_H */

