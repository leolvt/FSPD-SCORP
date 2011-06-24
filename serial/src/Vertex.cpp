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

};
