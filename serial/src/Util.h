#ifndef SCORP_UTIL_H
#define SCORP_UTIL_H

#include "Vertex.h"
#include "Edge.h"
#include <string>
#include <map>

namespace SCORP {

void parseInput(std::string filename, std::map<int, Vertex> v, VertexSet vs, 
        EdgeSet es);

};

#endif  /* SCORP_UTIL_H */

