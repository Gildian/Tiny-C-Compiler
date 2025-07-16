#include <iostream>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

// Constructor: Initialize the syntax analyzer with a lexical analyzer
SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer *l)
{
  la = l;
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
}

// Error handling: Display error message and exit
void SyntaxAnalyzer::error(TokenCodes tokenCodes)
{
  cout << endl;
  int spaces = this->la->getCurrentTokenPosition();
  string indent = "     ";
  for (int i = 0; i < spaces; i++)
  {
    indent += " ";
  }
  string tcs[] =
      {
          "PLUS", "MINUS", "TIMES", "SLASH", "MOD", "LPAREN", "RPAREN", "LBRACE",
          "RBRACE", "COMMA", "SEMICOLON", "OR", "AND", "ASSIGN", "EQL", "NOT", "NEQ",
          "LSS", "LEQ", "GTR", "GEQ", "BOOLSYM", "DOSYM", "ELSESYM", "FALSESYM",
          "FLOATSYM", "FORSYM", "IFSYM", "INTSYM", "PRINTFSYM", "RETURNSYM",
          "SCANFSYM", "TRUESYM", "VOIDSYM", "WHILESYM", "IDENT", "NUMLIT", "EOI",
          "NAL"};

  cout << indent << "^" << endl;
  cout << indent << "Error: " << tcs[tokenCodes] << " expected" << endl;
  exit(-1);
}

void SyntaxAnalyzer::Program()
{
  Functions();

  if (nextTokenCode != TokenCodes::EOI)
  {
    error(EOI);
  }
}

void SyntaxAnalyzer::Functions()
{
  while (nextTokenCode != TokenCodes::EOI)
  {
    Function();
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
}

void SyntaxAnalyzer::Function()
{
  if (nextTokenCode != TokenCodes::BOOLSYM &&
      nextTokenCode != TokenCodes::FLOATSYM &&
      nextTokenCode != TokenCodes::INTSYM &&
      nextTokenCode != TokenCodes::VOIDSYM)
  {
    error(TokenCodes::BOOLSYM);
  }
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  if (nextTokenCode != TokenCodes::IDENT)
  {
    error(IDENT);
  }
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  if (nextTokenCode != TokenCodes::LPAREN)
  {
    error(LPAREN);
  }

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  Paramlist();

  if (nextTokenCode != TokenCodes::RPAREN)
  {
    error(TokenCodes::RPAREN);
  }

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  Compstmt();
}

void SyntaxAnalyzer::Paramlist()
{
  if (nextTokenCode == TokenCodes::BOOLSYM ||
      nextTokenCode == TokenCodes::FLOATSYM ||
      nextTokenCode == TokenCodes::INTSYM)
  {
    Parameters();
  }
  // If no parameters, just continue (empty parameter list is valid)
}

void SyntaxAnalyzer::Parameters()
{
  Parameter();

  while (nextTokenCode == TokenCodes::COMMA)
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Parameter();
  }
}

void SyntaxAnalyzer::Parameter()
{
  if (nextTokenCode != TokenCodes::BOOLSYM &&
      nextTokenCode != TokenCodes::FLOATSYM &&
      nextTokenCode != TokenCodes::INTSYM)
  {
    error(TokenCodes::BOOLSYM);
  }

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  if (nextTokenCode != TokenCodes::IDENT)
  {
    error(TokenCodes::IDENT);
  }

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
}

void SyntaxAnalyzer::Compstmt()
{
  if (nextTokenCode != LBRACE)
  {
    error(LBRACE);
  }
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  Seqofstmt();

  if (nextTokenCode != RBRACE)
  {
    error(RBRACE);
  }
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
}

void SyntaxAnalyzer::Seqofstmt()
{
  while (nextTokenCode != RBRACE)
  {
    Statement();
  }
}

void SyntaxAnalyzer::Statement()
{
  switch (nextTokenCode)
  {
    //<STATEMENT> → <DECLARATION> ;
  case (BOOLSYM):
  case (FLOATSYM):
  case (INTSYM):
    Declaration();
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;

    //<STATEMENT> → do <BLOCK> while ( <EXPRESSION> ) ;
  case (DOSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Block();
    if (nextTokenCode != WHILESYM)
    {
      error(WHILESYM);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();

    if (nextTokenCode != LPAREN)
    {
      error(LPAREN);
    }
    Expression();
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != RPAREN)
    {
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;

  //<STATEMENT> → <EXPRESSION> ;

//<STATEMENT> → for ( <EXPRESSION> ; <EXPRESSION> ; <EXPRESSION> ) <BLOCK>
  case(FORSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if(nextTokenCode != LPAREN){
      error(LPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if(nextTokenCode != SEMICOLON){
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if(nextTokenCode != SEMICOLON){
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if(nextTokenCode != RPAREN){
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Block();
    break;

//<STATEMENT> → if ( <EXPRESSION> ) <BLOCK> and <STATEMENT> → if ( <EXPRESSION> ) <BLOCK> else <BLOCK> 
case(IFSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if(nextTokenCode != LPAREN){
      error(LPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if(nextTokenCode != RPAREN){
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Block();

    if(nextTokenCode == ELSESYM){
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      Block();
    }
    break;
  //<STATEMENT> → printf ( <IDENTLIST> ) ;
  case (PRINTFSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != LPAREN)
    {
      error(LPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != RPAREN)
    {
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;

  //<STATEMENT> → return <EXPRESSION> ;
  case (RETURNSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;

    //<STATEMENT> → scanf ( <IDENTLIST> ) ;
  case (SCANFSYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != LPAREN)
    {
      error(LPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != RPAREN)
    {
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;

  //<STATEMENT> → while ( <EXPRESSION> ) <BLOCK>
  case (WHILESYM):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode != LPAREN)
    {
      error(LPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if (nextTokenCode != RPAREN)
    {
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Block();
    break;
    
  // Handle assignment statements and expressions  
  case (IDENT):
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    if (nextTokenCode == ASSIGN)
    {
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      Expression();
    }
    else if (nextTokenCode == EQL)
    {
      // Handle equality comparison like "x == 10"
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      Expression();
    }
    else
    {
      // Handle other expressions that might start with an identifier
      // but since we already consumed the IDENT, we need to continue from where we are
      Or();
    }
    
    if (nextTokenCode != SEMICOLON)
    {
      error(SEMICOLON);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    break;
    
  default:
    error(IDENT);
    break;
  }
}

void SyntaxAnalyzer::Declaration()
{
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
  Identlist();
}

void SyntaxAnalyzer::Identlist()
{
  while (nextTokenCode != SEMICOLON)
  {
    if (nextTokenCode != IDENT)
    {
      error(IDENT);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();

    // Handle optional initialization
    if (nextTokenCode == ASSIGN)
    {
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      Expression();
    }

    if (nextTokenCode == SEMICOLON)
      break;

    if (nextTokenCode != COMMA)
    {
      error(COMMA);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
}

void SyntaxAnalyzer::Block()
{
  if (nextTokenCode == LBRACE)
  {
    Compstmt();
  }
  else
  {
    Statement();
  }
}

void SyntaxAnalyzer::Expression()
{
  Or();
}

void SyntaxAnalyzer::Or()
{
  And();
  OrPrime();
}
void SyntaxAnalyzer::OrPrime()
{
  if (nextTokenCode == OR)
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    And();
    OrPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::And()
{
  Equality();
  AndPrime();
}

void SyntaxAnalyzer::AndPrime()
{
  if (nextTokenCode == AND)
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Equality();
    AndPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::Equality()
{
  Relational();
  EqualityPrime();
}

void SyntaxAnalyzer::EqualityPrime()
{
  if ((nextTokenCode == EQL) || (nextTokenCode == NEQ))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Relational();
    EqualityPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::Relational()
{
  Term();
  RelationalPrime();
}

void SyntaxAnalyzer::RelationalPrime()
{
  if ((nextTokenCode == LSS) || (nextTokenCode == LEQ) || (nextTokenCode == GTR) || (nextTokenCode == GEQ))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Term();
    RelationalPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::Term()
{
  Factor();
  TermPrime();
}

void SyntaxAnalyzer::TermPrime()
{
  if ((nextTokenCode == PLUS) || (nextTokenCode == MINUS))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Factor();
    TermPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::Factor()
{
  Unary();
  FactorPrime();
}

void SyntaxAnalyzer::FactorPrime()
{
  if ((nextTokenCode == TIMES) || (nextTokenCode == SLASH) || (nextTokenCode == MOD))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Unary();
    FactorPrime();
  }
  else
  {
  }
}

void SyntaxAnalyzer::Unary()
{
  if (nextTokenCode == NOT)
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
  Primary();
}

void SyntaxAnalyzer::Primary()
{
  if (nextTokenCode == LPAREN)
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    Expression();
    if (nextTokenCode != RPAREN)
    {
      error(RPAREN);
    }
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
  else if ((nextTokenCode == IDENT) || (nextTokenCode == NUMLIT) || (nextTokenCode == TRUESYM) || (nextTokenCode == FALSESYM))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
  else
  {
    error(IDENT);
  }
}