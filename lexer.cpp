#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
#include "token.h"

using namespace std;

bool isLetter(char c) { return isalpha((unsigned char)c); }
bool isDigit(char c) { return isdigit((unsigned char)c); }

bool isKeyword(const string &w) {
    return (w == "int" || w == "print" || w == "if" || w == "while");
}

string keywordType(const string &w) {
    if (w == "int") return "INT";
    if (w == "print") return "PRINT";
    if (w == "if") return "IF";
    if (w == "while") return "WHILE";
    return "ID";
}

vector<Token> tokenize(istream &in) {
    vector<Token> tokens;
    string line;
    int lineNo = 0;

    while (getline(in, line)) {
        lineNo++;
        for (size_t i = 0; i < line.size();) {
            char c = line[i];

            if (isspace(c)) { i++; continue; }

            if (isLetter(c)) {
                string word;
                while (i < line.size() && (isLetter(line[i]) || isDigit(line[i])))
                    word += line[i++];
                tokens.push_back({isKeyword(word) ? keywordType(word) : "ID", word, lineNo});
                continue;
            }

            if (isDigit(c)) {
                string num;
                while (i < line.size() && isDigit(line[i]))
                    num += line[i++];
                tokens.push_back({"NUM", num, lineNo});
                continue;
            }

            if (c == '=' && i + 1 < line.size() && line[i+1] == '=') {
                tokens.push_back({"EQ", "==", lineNo});
                i += 2;
                continue;
            }

            switch (c) {
                case '+': tokens.push_back({"PLUS", "+", lineNo}); break;
                case '-': tokens.push_back({"MINUS", "-", lineNo}); break;
                case '*': tokens.push_back({"MULT", "*", lineNo}); break;
                case '/': tokens.push_back({"DIV", "/", lineNo}); break;
                case '=': tokens.push_back({"ASSIGN", "=", lineNo}); break;
                case ';': tokens.push_back({"SEMICOLON", ";", lineNo}); break;
                case '(': tokens.push_back({"LPAREN", "(", lineNo}); break;
                case ')': tokens.push_back({"RPAREN", ")", lineNo}); break;
                case '{': tokens.push_back({"LBRACE", "{", lineNo}); break;
                case '}': tokens.push_back({"RBRACE", "}", lineNo}); break;
                default: tokens.push_back({"INVALID", string(1, c), lineNo});
            }
            i++;
        }
    }

    tokens.push_back({"EOF", "EOF", lineNo});
    return tokens;
}

int main() {
    ifstream in("input.mc");
    ofstream out("tokens_output.txt");

    vector<Token> tokens = tokenize(in);
    for (auto &t : tokens)
        out << t.type << " " << t.lexeme << " " << t.line << "\n";

    return 0;
}
