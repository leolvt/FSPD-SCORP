#include <iostream>
#include "Attribute.h"
    using namespace std;
using namespace SCORP;

int main(int argc, const char *argv[]) {
    Attribute a = Attribute("aaa");
    Attribute b = Attribute("AAA");

    cout << (a < b) << endl;
    return 0;
}
