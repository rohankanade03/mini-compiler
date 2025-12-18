#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <iostream>
#include "symbol.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol> table;

public:
    bool declare(const std::string &name, const std::string &type, int line) {
        if (table.find(name) != table.end()) {
            std::cerr << "Semantic error at line " << line
                      << ": variable '" << name << "' already declared\n";
            return false;
        }
        table[name] = {name, type, line};
        return true;
    }

    bool exists(const std::string &name, int line) {
        if (table.find(name) == table.end()) {
            std::cerr << "Semantic error at line " << line
                      << ": variable '" << name << "' not declared\n";
            return false;
        }
        return true;
    }

    void print() const {
        std::cout << "\n--- Symbol Table ---\n";
        for (const auto &p : table) {
            std::cout << p.second.name
                      << " | type: " << p.second.type
                      << " | declared at line " << p.second.lineDeclared
                      << "\n";
        }
    }
};

#endif // SYMBOL_TABLE_H