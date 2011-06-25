#ifndef SCORP_VERTEX_H
#define SCORP_VERTEX_H

#include <set>
#include "Attribute.h"

namespace SCORP {

class Vertex {
    public:
        Vertex() {};
        Vertex(int id);
        ~Vertex();
        void addAttribute(const Attribute& a);
        void removeAttribute(const Attribute& a);
        bool hasAttribute(const Attribute& a);
    private:
        int id;
        std::set<Attribute> attributes;
};

class VertexSet {
    public:
        typedef std::set<int>::iterator iterator;
        VertexSet();
        VertexSet(std::set<int> s);
        ~VertexSet();
        void addVertex(int id);
        void removeVertex(int id);
        bool hasVertex(int id);
        int size();
        iterator begin();
        iterator end();
    private:
        std::set<int> vset;
};

};

#endif  /* SCORP_VERTEX_H */

