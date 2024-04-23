#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class SyntaxAnalyzer
{
  private:
    LexicalAnalyzer *la;
    Token *nextToken;
    TokenCodes nextTokenCode;

  public:
    SyntaxAnalyzer(LexicalAnalyzer*);
    void Unary();
    void FactorPrime();
    void Relational();
    void RelationalPrime();
    void Equality();
    void EqualityPrime();
    void And();
    void AndPrime();
    void Or();
    void OrPrime();
    void Parameter();
    void Parameters();
    void Functions();
    void Function();
    void Paramlist();
    void error(TokenCodes);
    void Program();
    void Declarations();
    void Declaration();
    void Identlist();
    void Compstmt();
    void Seqofstmt();
    void Block();
    void Statement();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void TermPrime();
};

#endif
