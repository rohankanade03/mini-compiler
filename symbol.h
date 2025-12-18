#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

struct Symbol {
    std::string name;
    std::string type;
    int lineDeclared;
};

#endif // SYMBOL_H
