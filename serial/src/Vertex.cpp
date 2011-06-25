#include <set>
#include "Vertex.h"
#include "Attribute.h"

namespace SCORP {

// =================== //

Vertex::Vertex(int id) {
    this->id = id;
    this->attributes = std::set<Attribute>();
}

// =================== //

Vertex::~Vertex() {
    this->attributes.clear();
}

// =================== //

void Vertex::addAttribute(const Attribute& a) {
    this->attributes.insert(a);
}

// =================== //

void Vertex::removeAttribute(const Attribute& a) {
    this->attributes.erase(a);
}

// =================== //

bool Vertex::hasAttribute(const Attribute& a) {
    this->attributes.find(a) != attributes.end();
}

// =================== //

VertexSet::VertexSet() {

}

// =================== //

VertexSet::VertexSet(std::set<int> s) {
    this->vset = s;
}

// =================== //

VertexSet::~VertexSet() {
    this->vset.clear();
}

// =================== //

void VertexSet::addVertex(int id) {
    this->vset.insert(id);
}

// =================== //

void VertexSet::removeVertex(int id) {
    this->vset.erase(id);
}

// =================== //

bool VertexSet::hasVertex(int id) {
    return vset.find(id) != vset.end();
}

// =================== //

int VertexSet::size() {
    return vset.size();
}

// =================== //

VertexSet::iterator VertexSet::begin() {
    return vset.begin();
}

// =================== //

VertexSet::iterator VertexSet::end() {
    return vset.end();
}

// =================== //

};
