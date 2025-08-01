#include <iostream>
#include <string>
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
  semanticAnalyzer = new SemanticAnalyzer();
  currentLine = 1;
}

// Destructor: Clean up semantic analyzer
SyntaxAnalyzer::~SyntaxAnalyzer()
{
  delete semanticAnalyzer;
}

// Error handling: Display user-friendly error message and exit
void SyntaxAnalyzer::error(TokenCodes expectedToken)
{
  cout << endl;
  cout << "===============================================" << endl;
  cout << "SYNTAX ERROR" << endl;
  cout << "===============================================" << endl;
  
  // Create user-friendly error messages
  string expectedDescription = getTokenDescription(expectedToken);
  string currentToken = nextToken ? nextToken->getLexemeString() : "end of file";
  
  cout << "Expected: " << expectedDescription << endl;
  
  if (nextToken && !nextToken->getLexemeString().empty()) {
    cout << "Found:    '" << currentToken << "'" << endl;
  } else {
    cout << "Found:    end of file" << endl;
  }
  
  cout << "Line:     " << currentLine << endl;
  cout << "===============================================" << endl;
  cout << "Please check your syntax and try again." << endl;
  cout << "===============================================" << endl;
  exit(-1);
}

// Helper function to convert token codes to user-friendly descriptions
string SyntaxAnalyzer::getTokenDescription(TokenCodes token)
{
  switch (token) {
    case PLUS: return "'+' (addition operator)";
    case MINUS: return "'-' (subtraction operator)";
    case TIMES: return "'*' (multiplication operator)";
    case SLASH: return "'/' (division operator)";
    case MOD: return "'%' (modulo operator)";
    case LPAREN: return "'(' (left parenthesis)";
    case RPAREN: return "')' (right parenthesis)";
    case LBRACE: return "'{' (left brace)";
    case RBRACE: return "'}' (right brace)";
    case LBRACKET: return "'[' (left bracket)";
    case RBRACKET: return "']' (right bracket)";
    case COMMA: return "',' (comma)";
    case SEMICOLON: return "';' (semicolon)";
    case OR: return "'||' (logical OR)";
    case AND: return "'&&' (logical AND)";
    case ASSIGN: return "'=' (assignment operator)";
    case EQL: return "'==' (equality operator)";
    case NOT: return "'!' (logical NOT)";
    case NEQ: return "'!=' (not equal operator)";
    case LSS: return "'<' (less than operator)";
    case LEQ: return "'<=' (less than or equal operator)";
    case GTR: return "'>' (greater than operator)";
    case GEQ: return "'>=' (greater than or equal operator)";
    case BOOLSYM: return "data type (int, float, bool, or void)";
    case DOSYM: return "'do' keyword";
    case ELSESYM: return "'else' keyword";
    case FALSESYM: return "'false' keyword";
    case FLOATSYM: return "'float' keyword";
    case FORSYM: return "'for' keyword";
    case IFSYM: return "'if' keyword";
    case INTSYM: return "'int' keyword";
    case PRINTFSYM: return "'printf' function";
    case RETURNSYM: return "'return' keyword";
    case SCANFSYM: return "'scanf' function";
    case TRUESYM: return "'true' keyword";
    case VOIDSYM: return "'void' keyword";
    case WHILESYM: return "'while' keyword";
    case IDENT: return "identifier (variable or function name)";
    case NUMLIT: return "number literal";
    case EOI: return "end of input";
    default: return "valid token";
  }
}

void SyntaxAnalyzer::Program()
{
  Functions();

  if (nextTokenCode != TokenCodes::EOI)
  {
    error(EOI);
  }
  
  // Print semantic analysis results
  printSemanticReport();
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
  TokenCodes returnType = nextTokenCode;
  
  if (nextTokenCode != TokenCodes::BOOLSYM &&
      nextTokenCode != TokenCodes::FLOATSYM &&
      nextTokenCode != TokenCodes::INTSYM &&
      nextTokenCode != TokenCodes::VOIDSYM)
  {
    error(TokenCodes::BOOLSYM);
  }
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  string functionName = "";
  if (nextTokenCode != TokenCodes::IDENT)
  {
    error(IDENT);
  }
  functionName = nextToken->getLexemeString();
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  if (nextTokenCode != TokenCodes::LPAREN)
  {
    error(LPAREN);
  }

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  // Enter function scope
  semanticAnalyzer->enterScope();
  
  // Parse parameters and collect their types
  vector<TokenCodes> paramTypes;
  Paramlist(paramTypes);

  if (nextTokenCode != TokenCodes::RPAREN)
  {
    error(TokenCodes::RPAREN);
  }

  // Declare function in symbol table
  semanticAnalyzer->declareFunction(functionName, returnType, paramTypes, currentLine);

  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();

  Compstmt();
  
  // Exit function scope
  semanticAnalyzer->exitScope();
}

void SyntaxAnalyzer::Paramlist(vector<TokenCodes>& paramTypes)
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
  TokenCodes paramType = nextTokenCode;
  
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

  string paramName = nextToken->getLexemeString();
  semanticAnalyzer->declareParameter(paramName, paramType, currentLine);

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
    {
      string varName = nextToken->getLexemeString();
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      
      if (nextTokenCode == LBRACKET)
      {
        // Array assignment: identifier[expression] = expression
        semanticAnalyzer->checkArrayAccess(varName, currentLine);
        
        nextToken = la->getNextToken();
        nextTokenCode = nextToken->getTokenCode();
        
        Expression(); // Parse the index expression
        
        if (nextTokenCode != RBRACKET)
        {
          error(RBRACKET);
        }
        nextToken = la->getNextToken();
        nextTokenCode = nextToken->getTokenCode();
        
        if (nextTokenCode == ASSIGN)
        {
          nextToken = la->getNextToken();
          nextTokenCode = nextToken->getTokenCode();
          Expression();
        }
        else
        {
          error(ASSIGN);
        }
      }
      else if (nextTokenCode == ASSIGN)
      {
        // Regular variable assignment
        semanticAnalyzer->checkVariableUsage(varName, currentLine);
        nextToken = la->getNextToken();
        nextTokenCode = nextToken->getTokenCode();
        Expression();
      }
      else if (nextTokenCode == EQL)
      {
        // Handle equality comparison like "x == 10"
        semanticAnalyzer->checkVariableUsage(varName, currentLine);
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
  TokenCodes varType = nextTokenCode;
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
  Identlist(varType);
}

void SyntaxAnalyzer::Identlist(TokenCodes varType)
{
  while (nextTokenCode != SEMICOLON)
  {
    if (nextTokenCode != IDENT)
    {
      error(IDENT);
    }
    
    string varName = nextToken->getLexemeString();
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();

    // Check if this is an array declaration
    if (nextTokenCode == LBRACKET)
    {
      ArrayDeclaration(varType, varName);
    }
    else
    {
      // Regular variable declaration
      semanticAnalyzer->declareVariable(varName, varType, currentLine);
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

void SyntaxAnalyzer::ArrayDeclaration(TokenCodes varType, const string& arrayName)
{
  // We're already at the LBRACKET token
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
  
  if (nextTokenCode != NUMLIT)
  {
    error(NUMLIT);
  }
  
  // Parse array size
  string sizeStr = nextToken->getLexemeString();
  int arraySize = 0;
  try {
    arraySize = std::stoi(sizeStr);
  } catch (const std::exception& e) {
    cout << "Error: Invalid array size '" << sizeStr << "'" << endl;
    exit(-1);
  }
  
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
  
  if (nextTokenCode != RBRACKET)
  {
    error(RBRACKET);
  }
  
  // Declare the array in the symbol table
  semanticAnalyzer->declareArray(arrayName, varType, arraySize, currentLine);
  
  nextToken = la->getNextToken();
  nextTokenCode = nextToken->getTokenCode();
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
  else if (nextTokenCode == IDENT)
  {
    string varName = nextToken->getLexemeString();
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
    
    // Check if this is array access
    if (nextTokenCode == LBRACKET)
    {
      // Array access: identifier[expression]
      semanticAnalyzer->checkArrayAccess(varName, currentLine);
      
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
      
      Expression(); // Parse the index expression
      
      if (nextTokenCode != RBRACKET)
      {
        error(RBRACKET);
      }
      nextToken = la->getNextToken();
      nextTokenCode = nextToken->getTokenCode();
    }
    else
    {
      // Regular variable access
      semanticAnalyzer->checkVariableUsage(varName, currentLine);
    }
  }
  else if ((nextTokenCode == NUMLIT) || (nextTokenCode == TRUESYM) || (nextTokenCode == FALSESYM))
  {
    nextToken = la->getNextToken();
    nextTokenCode = nextToken->getTokenCode();
  }
  else
  {
    error(IDENT);
  }
}

// Semantic analysis methods
SemanticAnalyzer* SyntaxAnalyzer::getSemanticAnalyzer() const
{
  return semanticAnalyzer;
}

void SyntaxAnalyzer::printSemanticReport() const
{
  semanticAnalyzer->printSemanticReport();
  semanticAnalyzer->printSymbolTable();
}