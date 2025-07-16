#include <iostream>
#include <fstream>
#include <cctype>

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(ifstream *scf) : sourceCodeFile(scf), currentTokenPosition(0) {}

Token *LexicalAnalyzer::getNextToken()
{
    Token *t = new Token();

    if (sourceCodeFile->eof())
    {
        t->setLexemeString("end of file");
        t->setTokenCode(TokenCodes::EOI);
        return t;
    }

    string newToken = "";
    char tokenPart = ' ';

    // Skip whitespace
    while (!sourceCodeFile->eof() && isWhitespace(tokenPart))
    {
        sourceCodeFile->get(tokenPart);
        if (sourceCodeFile->eof()) 
        {
            t->setLexemeString("end of file");
            t->setTokenCode(TokenCodes::EOI);
            return t;
        }
    }

    // Build the token
    if (!sourceCodeFile->eof())
    {
        newToken += tokenPart;
        
        // Check for two-character operators
        if (!sourceCodeFile->eof())
        {
            char peekedToken = sourceCodeFile->peek();
            string nextCharacter(1, peekedToken);
            
            if (LexicalAnalyzer::checkLongLexeme(newToken, nextCharacter))
            {
                sourceCodeFile->get(tokenPart);
                newToken += tokenPart;
            }
            else
            {
                // Continue building token for identifiers and numbers
                while (!sourceCodeFile->eof())
                {
                    char peeked = sourceCodeFile->peek();
                    
                    // Stop if we hit whitespace or EOF
                    if (isWhitespace(peeked) || sourceCodeFile->eof())
                        break;
                        
                    // Stop if next character would start a new token
                    string peekStr(1, peeked);
                    if (LexicalAnalyzer::checkNextCharacter(newToken, peekStr, LexicalAnalyzer::lexemes))
                        break;
                        
                    // For identifiers and numbers, continue building
                    if (isAlpha(newToken[0]) || isDigit(newToken[0]))
                    {
                        if (isAlpha(peeked) || isDigit(peeked) ||
                            (peeked == '.' && isDigit(newToken[0])))
                        {
                            sourceCodeFile->get(tokenPart);
                            newToken += tokenPart;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    cout << newToken << " ";
    t->setLexemeString(newToken);

    if (newToken == "bool") t->setTokenCode(BOOLSYM);
    else if (newToken == "&&") t->setTokenCode(AND);
    else if (newToken == "||") t->setTokenCode(OR);
    else if (newToken == ",") t->setTokenCode(COMMA);
    else if (newToken == "scanf") t->setTokenCode(SCANFSYM);
    else if (newToken == "return") t->setTokenCode(RETURNSYM);
    else if (newToken == "printf") t->setTokenCode(PRINTFSYM);
    else if (newToken == "if") t->setTokenCode(IFSYM);
    else if (newToken == "int") t->setTokenCode(INTSYM);
    else if (newToken == "void") t->setTokenCode(VOIDSYM);
    else if (newToken == "for") t->setTokenCode(FORSYM);
    else if (newToken == "float") t->setTokenCode(FLOATSYM);
    else if (newToken == "(") t->setTokenCode(LPAREN);
    else if (newToken == ")") t->setTokenCode(RPAREN);
    else if (newToken == "==") t->setTokenCode(EQL);
    else if (newToken == "!=") t->setTokenCode(NEQ);
    else if (newToken == "do") t->setTokenCode(DOSYM);
    else if (newToken == "<") t->setTokenCode(LSS);
    else if (newToken == "{") t->setTokenCode(LBRACE);
    else if (newToken == "}") t->setTokenCode(RBRACE);
    else if (newToken == "<=") t->setTokenCode(LEQ);
    else if (newToken == ">") t->setTokenCode(GTR);
    else if (newToken == ">=") t->setTokenCode(GEQ);
    else if (newToken == "false") t->setTokenCode(FALSESYM);
    else if (newToken == "while") t->setTokenCode(WHILESYM);
    else if (newToken == "+") t->setTokenCode(PLUS);
    else if (newToken == "-") t->setTokenCode(MINUS);
    else if (newToken == "*") t->setTokenCode(TIMES);
    else if (newToken == "/") t->setTokenCode(SLASH);
    else if (newToken == ";") t->setTokenCode(SEMICOLON);
    else if (newToken == "=") t->setTokenCode(ASSIGN);
    else if (newToken == "!") t->setTokenCode(NOT);
    else if (newToken == "%") t->setTokenCode(MOD);
    else if (newToken == "true") t->setTokenCode(TRUESYM);
    else
    {
        t->setTokenCode(LexicalAnalyzer::tokenCodeSolver(newToken, LexicalAnalyzer::lexemes));
    }
    currentTokenPosition += newToken.length();

    return t;
}

bool LexicalAnalyzer::checkNextCharacter(string &newToken, string &nextToken, string lexemes[])
{
    for (int i = 0; i < LEXEME_COUNT; i++)
    {
        if (lexemes[i] == nextToken)
        {
            if (nextToken == "." && isDigit(newToken[0]))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}

bool LexicalAnalyzer::validLexeme(string &token, char tokenpart, string lexemes[])
{
    for (int i = 0; i < LEXEME_COUNT; i++)
    {
        if (lexemes[i] == token)
        {
            // Don't allow keywords to be extended with alphabetic characters
            if (isAlpha(token[0]) && (isAlpha(tokenpart) || isDigit(tokenpart)))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}

bool LexicalAnalyzer::checkLongLexeme(string &token, string &nextToken)
{
    if (token == ":")
    {
        if (nextToken == "=")
        {
            return true;
        }
    }
    else if (token == "<")
    {
        if (nextToken == "=" || nextToken == ">")
        {
            return true;
        }
    }
    else if (token == ">")
    {
        if (nextToken == "=")
        {
            return true;
        }
    }
    else if (token == "!")
    {
        if (nextToken == "=")
        {
            return true;
        }
    }
    else if (token == "=")
    {
        if (nextToken == "=")
        {
            return true;
        }
    }
    else if (token == "&")
    {
        if (nextToken == "&")
        {
            return true;
        }
    }
    else if (token == "|")
    {
        if (nextToken == "|")
        {
            return true;
        }
    }
    return false;
}

TokenCodes LexicalAnalyzer::tokenCodeSolver(string &token, string lexemes[])
{
    for (int i = 0; i < LEXEME_COUNT; i++)
    {
        if (lexemes[i] == token)
        {
            TokenCodes result = static_cast<TokenCodes>(i);
            return result;
        }
    }
    if (isAlpha(token[0]))
    {
        for (size_t i = 0; i < token.length(); i++)
        {
            if (!isDigit(token[i]) && !isAlpha(token[i]))
            {
                return TokenCodes::NAL;
            }
        }
        return TokenCodes::IDENT;
    }
    else if (isDigit(token[0]))
    {
        bool hasDecimal = false;
        for (size_t i = 0; i < token.length(); i++)
        {
            if (!isDigit(token[i]))
            {
                if (token[i] == '.' && !hasDecimal)  // '.' character
                {
                    hasDecimal = true;
                    continue;
                }
                else
                {
                    return TokenCodes::NAL;
                }
            }
        }
        return TokenCodes::NUMLIT;
    }
    else
    {
        return TokenCodes::NAL;
    }
}

int LexicalAnalyzer::getCurrentTokenPosition()
{
    return currentTokenPosition;
}

bool LexicalAnalyzer::isAlpha(char c) const
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LexicalAnalyzer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

bool LexicalAnalyzer::isWhitespace(char c) const
{
    return c <= 32 && c != EOF;
}