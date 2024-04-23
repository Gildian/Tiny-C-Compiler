#include <iostream>
#include <fstream>

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

    while (!sourceCodeFile->eof() && tokenPart <= 32)
    {
        sourceCodeFile->get(tokenPart);
        tokenPart = tolower(tokenPart);
    }

    while (!sourceCodeFile->eof() && !LexicalAnalyzer::validLexeme(newToken, tokenPart, LexicalAnalyzer::lexemes))
    {
        newToken += tokenPart;
        char peekedToken = sourceCodeFile->peek();
        string nextCharacter(1, peekedToken);

        if (tokenPart == 10)
        {
            cout << endl;
        }

        if (LexicalAnalyzer::checkLongLexeme(newToken, nextCharacter))
        {
            sourceCodeFile->get(tokenPart);
            tokenPart = tolower(tokenPart);
            newToken += tokenPart;
            break;
        }

        if (LexicalAnalyzer::checkNextCharacter(newToken, nextCharacter, LexicalAnalyzer::lexemes) || peekedToken <= 32)
        {
            break;
        }

        sourceCodeFile->get(tokenPart);
        tokenPart = tolower(tokenPart);
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
    for (int i = 0; i < 35; i++)
    {
        if (lexemes[i] == nextToken)
        {
            if (nextToken == "." && (newToken[0] >= 48 && newToken[0] <= 57))
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
    for (int i = 0; i < 35; i++)
    {
        if (lexemes[i] == token)
        {
            if (token[0] >= 97 && token[0] <= 122 && tokenpart >= 97 && tokenpart <= 122)
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
    return false;
}

TokenCodes LexicalAnalyzer::tokenCodeSolver(string &token, string lexemes[])
{
    for (int i = 0; i < 35; i++)
    {
        if (lexemes[i] == token)
        {
            TokenCodes result = static_cast<TokenCodes>(i);
            return result;
        }
    }
    if (token[0] >= 97 && token[0] <= 122)
    {
        for (int i = 0; i < token.length(); i++)
        {
            if ((token[i] < 48 || token[i] > 57) && (token[i] < 97 || token[i] > 122))
            {
                return TokenCodes::NAL;
            }
        }
        return TokenCodes::IDENT;
    }
    else if (token[0] >= 48 && token[0] <= 57)
    {
        for (int i = 0; i < token.length(); i++)
        {
            if (token[i] < 48 || token[i] > 57)
            {
                if (token[i] == 46)
                {
                    i++;
                    for (i = 0; i < token.length(); i++)
                    {
                        if (token[i] < 48 || token[i] > 57)
                        {
                            return TokenCodes::NAL;
                        }
                    }
                    return TokenCodes::NUMLIT;
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