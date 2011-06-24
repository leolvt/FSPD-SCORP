#include <string>
#include "Attribute.h"

namespace SCORP {

// ========================== //

Attribute::Attribute(std::string value) {
    this->value = value;
}

// ========================== //

Attribute::~Attribute() {

}

// ========================== //

bool Attribute::operator==(const Attribute& a) const {
    return a.value == this->value;
}

// ========================== //

bool Attribute::operator<(const Attribute& a) const {
    return value < a.value;
}

// ========================== //

};
