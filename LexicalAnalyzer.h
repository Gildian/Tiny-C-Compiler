#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>

#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class LexicalAnalyzer
{
private:
    string lexemes[35] = {"+", "-", "*", "/", "%", "(", ")", "{", "}", ",", ";", "||", "&&",
                        "=", "==", "!", "!=", "<", "<=", ">", ">=", "bool", "do", "else", "false", "float", "for",
                        "if", "int", "printf", "return", "scanf", "true", "void", "while"};
  ifstream *sourceCodeFile;
  bool validLexeme(string &, char, string[]);
  bool checkLongLexeme(string &, string &);
  TokenCodes tokenCodeSolver(string &, string[]);
  bool checkNextCharacter(string &, string &, string[]);
  int currentTokenPosition;

public:
  LexicalAnalyzer(ifstream *);
  Token *getNextToken();
  int getCurrentTokenPosition();
};

#endif
