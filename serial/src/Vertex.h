#ifndef SCORP_VERTEX_H
#define SCORP_VERTEX_H

#include <set>
#include "Attribute.h"

namespace SCORP {

class Vertex {
    public:
        Vertex(int id);
        ~Vertex();
        void addAttribute(const Attribute& a);
        void removeAttribute(const Attribute& a);
    private:
        int id;
        std::set<Attribute> attributes;
};

};

#endif  /* SCORP_VERTEX_H */

