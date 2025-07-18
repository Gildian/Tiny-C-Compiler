#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <vector>
#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"
#include "SemanticAnalyzer.h"

using namespace std;

class SyntaxAnalyzer
{
  private:
    LexicalAnalyzer *la;
    Token *nextToken;
    TokenCodes nextTokenCode;
    SemanticAnalyzer *semanticAnalyzer;
    int currentLine;

  public:
    SyntaxAnalyzer(LexicalAnalyzer*);
    ~SyntaxAnalyzer();
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
    void Paramlist(vector<TokenCodes>& paramTypes);
    void error(TokenCodes);
    void Program();
    void Declarations();
    void Declaration();
    void Identlist(TokenCodes varType);
    void Compstmt();
    void Seqofstmt();
    void Block();
    void Statement();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void TermPrime();
    
    // Semantic analysis methods
    SemanticAnalyzer* getSemanticAnalyzer() const;
    void printSemanticReport() const;
    
private:
    // Helper method for better error messages
    string getTokenDescription(TokenCodes token);
};

#endif
