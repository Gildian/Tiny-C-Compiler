#include "SymbolTable.h"

SymbolTable::SymbolTable() : currentScope(0) {
    // Create global scope
    scopes.push_back(map<string, SymbolInfo>());
}

SymbolTable::~SymbolTable() {
    scopes.clear();
}

void SymbolTable::enterScope() {
    currentScope++;
    scopes.push_back(map<string, SymbolInfo>());
    cout << "Entered scope level " << currentScope << endl;
}

void SymbolTable::exitScope() {
    if (currentScope > 0) {
        cout << "Exiting scope level " << currentScope << endl;
        scopes.pop_back();
        currentScope--;
    }
}

int SymbolTable::getCurrentScope() const {
    return currentScope;
}

bool SymbolTable::insertSymbol(const string& name, DataType type, SymbolType symbolType, int lineNumber) {
    if (isDeclaredInCurrentScope(name)) {
        cout << "Error: Symbol '" << name << "' already declared in current scope at line " << lineNumber << endl;
        return false;
    }
    
    SymbolInfo symbol(name, type, symbolType, currentScope, lineNumber);
    scopes[currentScope][name] = symbol;
    
    cout << "Inserted " << symbolTypeToString(symbolType) << " '" << name 
         << "' of type " << dataTypeToString(type) << " at line " << lineNumber << endl;
    
    return true;
}

bool SymbolTable::insertFunction(const string& name, DataType returnType, const vector<DataType>& paramTypes, int lineNumber) {
    if (isDeclaredInCurrentScope(name)) {
        cout << "Error: Function '" << name << "' already declared in current scope at line " << lineNumber << endl;
        return false;
    }
    
    SymbolInfo symbol(name, returnType, SymbolType::FUNCTION, currentScope, lineNumber);
    symbol.parameterTypes = paramTypes;
    symbol.parameterCount = paramTypes.size();
    
    scopes[currentScope][name] = symbol;
    
    cout << "Inserted function '" << name << "' with return type " << dataTypeToString(returnType) 
         << " and " << paramTypes.size() << " parameters at line " << lineNumber << endl;
    
    return true;
}

SymbolInfo* SymbolTable::lookupSymbol(const string& name) {
    // Search from current scope back to global scope
    for (int i = currentScope; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) {
            return &(it->second);
        }
    }
    return nullptr;
}

SymbolInfo* SymbolTable::lookupInCurrentScope(const string& name) {
    auto it = scopes[currentScope].find(name);
    if (it != scopes[currentScope].end()) {
        return &(it->second);
    }
    return nullptr;
}

bool SymbolTable::isDeclared(const string& name) {
    return lookupSymbol(name) != nullptr;
}

bool SymbolTable::isDeclaredInCurrentScope(const string& name) {
    return lookupInCurrentScope(name) != nullptr;
}

DataType SymbolTable::getSymbolType(const string& name) {
    SymbolInfo* symbol = lookupSymbol(name);
    return symbol ? symbol->dataType : DataType::UNKNOWN;
}

void SymbolTable::printSymbolTable() const {
    cout << "\n=== Symbol Table ===" << endl;
    for (int i = 0; i <= currentScope; i++) {
        cout << "Scope " << i << ":" << endl;
        for (const auto& pair : scopes[i]) {
            const SymbolInfo& symbol = pair.second;
            cout << "  " << symbol.name << " (" << symbolTypeToString(symbol.symbolType) 
                 << ", " << dataTypeToString(symbol.dataType) << ", line " << symbol.lineNumber << ")";
            if (symbol.symbolType == SymbolType::FUNCTION) {
                cout << " - " << symbol.parameterCount << " parameters";
            }
            cout << endl;
        }
    }
    cout << "===================" << endl;
}

void SymbolTable::printCurrentScope() const {
    cout << "Current scope (" << currentScope << "): ";
    for (const auto& pair : scopes[currentScope]) {
        cout << pair.first << " ";
    }
    cout << endl;
}

DataType SymbolTable::tokenToDataType(TokenCodes token) {
    switch (token) {
        case INTSYM: return DataType::INT;
        case FLOATSYM: return DataType::FLOAT;
        case BOOLSYM: return DataType::BOOL;
        case VOIDSYM: return DataType::VOID;
        default: return DataType::UNKNOWN;
    }
}

string SymbolTable::dataTypeToString(DataType type) {
    switch (type) {
        case DataType::INT: return "int";
        case DataType::FLOAT: return "float";
        case DataType::BOOL: return "bool";
        case DataType::VOID: return "void";
        case DataType::UNKNOWN: return "unknown";
        default: return "unknown";
    }
}

string SymbolTable::symbolTypeToString(SymbolType type) {
    switch (type) {
        case SymbolType::VARIABLE: return "variable";
        case SymbolType::FUNCTION: return "function";
        case SymbolType::PARAMETER: return "parameter";
        default: return "unknown";
    }
}
