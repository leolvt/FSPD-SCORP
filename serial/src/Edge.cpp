#include "Edge.h"

namespace SCORP {

// =================== //

Edge::Edge(int from, int to) {
    if (from > to) {
        this->from = to;
        this->to = from;
    }
    else {
        this->from = from;
        this->to = to;
    }
}

// =================== //

bool Edge::operator==(const Edge& e) const {
    return (e.from == from && e.to == to);
}

// =================== //

bool Edge::operator<(const Edge& e) const {
    return (from < e.from || (from == e.from && to < e.to));
//  return (from < e.from) ? true : ((from == e.from) ? (to < e.to) : false);
}

// =================== //

EdgeSet::EdgeSet() {

}

// =================== //

EdgeSet::EdgeSet(std::set<Edge> s) {
    this->eset = s;
}

// =================== //

EdgeSet::~EdgeSet() {

}

// =================== //

void EdgeSet::addEdge(const Edge& e ) {
    eset.insert(e);
}

// =================== //

}
