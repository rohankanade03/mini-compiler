#include <iostream>
#include <fstream>
#include <vector>
#include "token.h"
#include "symbol_table.h"

using namespace std;

vector<Token> tokens;
int currentIndex = 0;
bool hadError = false;
SymbolTable symTable;

Token currentToken() { return tokens[currentIndex]; }
void advance() { if (currentIndex < tokens.size()) currentIndex++; }

bool match(const string &t) {
    if (currentToken().type == t) { advance(); return true; }
    return false;
}

void error(const string &msg) {
    hadError = true;
    cerr << "Syntax error at line " << currentToken().line
         << ": " << msg << endl;
    advance();
}

void parseExpression();
void parseStatement();

void parseDeclaration() {
    match("INT");
    Token id = currentToken();
    match("ID");
    symTable.declare(id.lexeme, "int", id.line);
    match("SEMICOLON");
}

void parseAssignment() {
    Token id = currentToken();
    match("ID");
    symTable.exists(id.lexeme, id.line);
    match("ASSIGN");
    parseExpression();
    match("SEMICOLON");
}

void parsePrint() {
    match("PRINT");
    match("LPAREN");
    Token id = currentToken();
    match("ID");
    symTable.exists(id.lexeme, id.line);
    match("RPAREN");
    match("SEMICOLON");
}

void parseFactor() {
    if (currentToken().type == "NUM") advance();
    else if (currentToken().type == "ID") {
        symTable.exists(currentToken().lexeme, currentToken().line);
        advance();
    } else error("Expected number or identifier");
}

void parseExpression() {
    parseFactor();
    while (currentToken().type == "PLUS" || currentToken().type == "MINUS") {
        advance();
        parseFactor();
    }
}

void parseStatement() {
    if (currentToken().type == "INT") parseDeclaration();
    else if (currentToken().type == "ID") parseAssignment();
    else if (currentToken().type == "PRINT") parsePrint();
    else if (currentToken().type == "EOF") return;
    else error("Unknown statement");
}

int main() {
    ifstream in("tokens_output.txt");
    string type, lexeme;
    int line;

    while (in >> type >> lexeme >> line)
        tokens.push_back({type, lexeme, line});

    while (currentToken().type != "EOF")
        parseStatement();

    if (!hadError) cout << "Parsing successful\n";
    symTable.print();

    return 0;
}
