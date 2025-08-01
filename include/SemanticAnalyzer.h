#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "SymbolTable.h"
#include "TokenCodes.h"
#include <string>
#include <vector>

using namespace std;

class SemanticAnalyzer {
private:
    SymbolTable* symbolTable;
    vector<string> errors;
    vector<string> warnings;
    int currentLine;
    
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer();
    
    // Symbol table operations
    void enterScope();
    void exitScope();
    bool declareVariable(const string& name, TokenCodes type, int line);
    bool declareArray(const string& name, TokenCodes type, int size, int line);
    bool declareFunction(const string& name, TokenCodes returnType, const vector<TokenCodes>& paramTypes, int line);
    bool declareParameter(const string& name, TokenCodes type, int line);
    
    // Semantic checks
    bool checkVariableUsage(const string& name, int line);
    bool checkArrayAccess(const string& name, int line);
    bool checkFunctionCall(const string& name, int paramCount, int line);
    bool checkAssignment(const string& varName, DataType expressionType, int line);
    bool checkReturnType(DataType returnType, DataType expectedType, int line);
    
    // Type checking
    DataType getVariableType(const string& name);
    DataType checkBinaryOperation(DataType left, DataType right, TokenCodes op, int line);
    DataType checkUnaryOperation(DataType operand, TokenCodes op, int line);
    
    // Error and warning management
    void addError(const string& message);
    void addWarning(const string& message);
    bool hasErrors() const;
    bool hasWarnings() const;
    void printErrors() const;
    void printWarnings() const;
    void printSemanticReport() const;
    
    // Utility functions
    void setCurrentLine(int line);
    void printSymbolTable() const;
    
private:
    // Helper functions
    bool isNumericType(DataType type);
    bool isCompatibleTypes(DataType type1, DataType type2);
    DataType getPromotedType(DataType type1, DataType type2);
    string formatError(const string& message, int line);
    string formatWarning(const string& message, int line);
};

#endif
