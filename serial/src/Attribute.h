#ifndef SCORP_ATTRIBUTE_H
#define SCORP_ATTRIBUTE_H

#include <string>

namespace SCORP {

class Attribute {
    public:
        Attribute(std::string value);
        ~Attribute();
        bool operator==(const Attribute& a) const;;
        bool operator<(const Attribute& a) const;
    private:
        std::string value;
};

};

#endif  /* SCORP_ATTRIBUTE_H */

