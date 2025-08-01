#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <string>

#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class LexicalAnalyzer
{
private:
    static const int LEXEME_COUNT = 37;
    string lexemes[LEXEME_COUNT] = {"+", "-", "*", "/", "%", "(", ")", "{", "}", ",", ";", "||", "&&",
                        "=", "==", "!", "!=", "<", "<=", ">", ">=", "bool", "do", "else", "false", "float", "for",
                        "if", "int", "printf", "return", "scanf", "true", "void", "while", "[", "]"};
  ifstream *sourceCodeFile;
  bool validLexeme(string &, char, string[]);
  bool checkLongLexeme(string &, string &);
  TokenCodes tokenCodeSolver(string &, string[]);
  bool checkNextCharacter(string &, string &, string[]);
  int currentTokenPosition;
  
  // Helper functions for better code organization
  bool isAlpha(char c) const;
  bool isDigit(char c) const;
  bool isWhitespace(char c) const;

public:
  LexicalAnalyzer(ifstream *);
  Token *getNextToken();
  int getCurrentTokenPosition();
};

#endif
