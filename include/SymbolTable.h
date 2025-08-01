#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "TokenCodes.h"

using namespace std;

enum class DataType {
    INT,
    FLOAT,
    BOOL,
    VOID,
    UNKNOWN
};

enum class SymbolType {
    VARIABLE,
    FUNCTION,
    PARAMETER
};

struct SymbolInfo {
    string name;
    DataType dataType;
    SymbolType symbolType;
    int scopeLevel;
    int lineNumber;
    bool isInitialized;
    
    // Array-specific information
    bool isArray;
    int arraySize;
    
    // Function-specific information
    vector<DataType> parameterTypes;
    int parameterCount;
    
    SymbolInfo() : dataType(DataType::UNKNOWN), symbolType(SymbolType::VARIABLE), 
                   scopeLevel(0), lineNumber(0), isInitialized(false), 
                   isArray(false), arraySize(0), parameterCount(0) {}
    
    SymbolInfo(string n, DataType dt, SymbolType st, int scope, int line) 
        : name(n), dataType(dt), symbolType(st), scopeLevel(scope), 
          lineNumber(line), isInitialized(false), isArray(false), 
          arraySize(0), parameterCount(0) {}
};

class SymbolTable {
private:
    vector<map<string, SymbolInfo>> scopes;
    int currentScope;
    
public:
    SymbolTable();
    ~SymbolTable();
    
    // Scope management
    void enterScope();
    void exitScope();
    int getCurrentScope() const;
    
    // Symbol operations
    bool insertSymbol(const string& name, DataType type, SymbolType symbolType, int lineNumber);
    bool insertArray(const string& name, DataType type, int size, int lineNumber);
    bool insertFunction(const string& name, DataType returnType, const vector<DataType>& paramTypes, int lineNumber);
    SymbolInfo* lookupSymbol(const string& name);
    SymbolInfo* lookupInCurrentScope(const string& name);
    
    // Utility functions
    bool isDeclared(const string& name);
    bool isDeclaredInCurrentScope(const string& name);
    DataType getSymbolType(const string& name);
    
    // Debug and display
    void printSymbolTable() const;
    void printCurrentScope() const;
    
    // Type conversion utilities
    static DataType tokenToDataType(TokenCodes token);
    static string dataTypeToString(DataType type);
    static string symbolTypeToString(SymbolType type);
};

#endif
