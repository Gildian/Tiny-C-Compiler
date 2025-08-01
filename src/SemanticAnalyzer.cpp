#include "SemanticAnalyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() : currentLine(0) {
    symbolTable = new SymbolTable();
}

SemanticAnalyzer::~SemanticAnalyzer() {
    delete symbolTable;
}

void SemanticAnalyzer::enterScope() {
    symbolTable->enterScope();
}

void SemanticAnalyzer::exitScope() {
    symbolTable->exitScope();
}

bool SemanticAnalyzer::declareVariable(const string& name, TokenCodes type, int line) {
    setCurrentLine(line);
    DataType dataType = SymbolTable::tokenToDataType(type);
    
    if (dataType == DataType::UNKNOWN) {
        addError("Invalid data type for variable '" + name + "'");
        return false;
    }
    
    if (dataType == DataType::VOID) {
        addError("Variable '" + name + "' cannot be of type void");
        return false;
    }
    
    return symbolTable->insertSymbol(name, dataType, SymbolType::VARIABLE, line);
}

bool SemanticAnalyzer::declareArray(const string& name, TokenCodes type, int size, int line) {
    setCurrentLine(line);
    DataType dataType = SymbolTable::tokenToDataType(type);
    
    if (dataType == DataType::UNKNOWN) {
        addError("Invalid data type for array '" + name + "'");
        return false;
    }
    
    if (dataType == DataType::VOID) {
        addError("Array '" + name + "' cannot be of type void");
        return false;
    }
    
    if (size <= 0) {
        addError("Array '" + name + "' must have positive size");
        return false;
    }
    
    return symbolTable->insertArray(name, dataType, size, line);
}

bool SemanticAnalyzer::declareFunction(const string& name, TokenCodes returnType, const vector<TokenCodes>& paramTypes, int line) {
    setCurrentLine(line);
    DataType returnDataType = SymbolTable::tokenToDataType(returnType);
    
    if (returnDataType == DataType::UNKNOWN) {
        addError("Invalid return type for function '" + name + "'");
        return false;
    }
    
    vector<DataType> paramDataTypes;
    for (TokenCodes paramType : paramTypes) {
        DataType paramDataType = SymbolTable::tokenToDataType(paramType);
        if (paramDataType == DataType::UNKNOWN) {
            addError("Invalid parameter type in function '" + name + "'");
            return false;
        }
        if (paramDataType == DataType::VOID) {
            addError("Parameter cannot be of type void in function '" + name + "'");
            return false;
        }
        paramDataTypes.push_back(paramDataType);
    }
    
    return symbolTable->insertFunction(name, returnDataType, paramDataTypes, line);
}

bool SemanticAnalyzer::declareParameter(const string& name, TokenCodes type, int line) {
    setCurrentLine(line);
    DataType dataType = SymbolTable::tokenToDataType(type);
    
    if (dataType == DataType::UNKNOWN) {
        addError("Invalid data type for parameter '" + name + "'");
        return false;
    }
    
    if (dataType == DataType::VOID) {
        addError("Parameter '" + name + "' cannot be of type void");
        return false;
    }
    
    return symbolTable->insertSymbol(name, dataType, SymbolType::PARAMETER, line);
}

bool SemanticAnalyzer::checkVariableUsage(const string& name, int line) {
    setCurrentLine(line);
    
    if (!symbolTable->isDeclared(name)) {
        addError("Undeclared variable '" + name + "'");
        return false;
    }
    
    SymbolInfo* symbol = symbolTable->lookupSymbol(name);
    if (symbol && symbol->symbolType == SymbolType::FUNCTION) {
        addError("'" + name + "' is a function, not a variable");
        return false;
    }
    
    return true;
}

bool SemanticAnalyzer::checkArrayAccess(const string& name, int line) {
    setCurrentLine(line);
    
    if (!symbolTable->isDeclared(name)) {
        addError("Undeclared array '" + name + "'");
        return false;
    }
    
    SymbolInfo* symbol = symbolTable->lookupSymbol(name);
    if (!symbol) {
        addError("Symbol '" + name + "' not found");
        return false;
    }
    
    if (symbol->symbolType == SymbolType::FUNCTION) {
        addError("'" + name + "' is a function, not an array");
        return false;
    }
    
    if (!symbol->isArray) {
        addError("'" + name + "' is not an array");
        return false;
    }
    
    return true;
}

bool SemanticAnalyzer::checkFunctionCall(const string& name, int paramCount, int line) {
    setCurrentLine(line);
    
    if (!symbolTable->isDeclared(name)) {
        addError("Undeclared function '" + name + "'");
        return false;
    }
    
    SymbolInfo* symbol = symbolTable->lookupSymbol(name);
    if (!symbol || symbol->symbolType != SymbolType::FUNCTION) {
        addError("'" + name + "' is not a function");
        return false;
    }
    
    if (symbol->parameterCount != paramCount) {
        addError("Function '" + name + "' expects " + to_string(symbol->parameterCount) + 
                " parameters, but " + to_string(paramCount) + " provided");
        return false;
    }
    
    return true;
}

bool SemanticAnalyzer::checkAssignment(const string& varName, DataType expressionType, int line) {
    setCurrentLine(line);
    
    if (!checkVariableUsage(varName, line)) {
        return false;
    }
    
    DataType varType = getVariableType(varName);
    if (!isCompatibleTypes(varType, expressionType)) {
        addError("Type mismatch in assignment to '" + varName + "': cannot assign " + 
                SymbolTable::dataTypeToString(expressionType) + " to " + 
                SymbolTable::dataTypeToString(varType));
        return false;
    }
    
    if (varType != expressionType) {
        addWarning("Implicit type conversion in assignment to '" + varName + "' from " + 
                  SymbolTable::dataTypeToString(expressionType) + " to " + 
                  SymbolTable::dataTypeToString(varType));
    }
    
    return true;
}

bool SemanticAnalyzer::checkReturnType(DataType returnType, DataType expectedType, int line) {
    setCurrentLine(line);
    
    if (!isCompatibleTypes(expectedType, returnType)) {
        addError("Return type mismatch: expected " + SymbolTable::dataTypeToString(expectedType) + 
                ", but got " + SymbolTable::dataTypeToString(returnType));
        return false;
    }
    
    if (expectedType != returnType) {
        addWarning("Implicit type conversion in return statement from " + 
                  SymbolTable::dataTypeToString(returnType) + " to " + 
                  SymbolTable::dataTypeToString(expectedType));
    }
    
    return true;
}

DataType SemanticAnalyzer::getVariableType(const string& name) {
    return symbolTable->getSymbolType(name);
}

DataType SemanticAnalyzer::checkBinaryOperation(DataType left, DataType right, TokenCodes op, int line) {
    setCurrentLine(line);
    
    // Arithmetic operations
    if (op == PLUS || op == MINUS || op == TIMES || op == SLASH || op == MOD) {
        if (!isNumericType(left) || !isNumericType(right)) {
            addError("Arithmetic operation requires numeric operands");
            return DataType::UNKNOWN;
        }
        return getPromotedType(left, right);
    }
    
    // Comparison operations
    if (op == LSS || op == LEQ || op == GTR || op == GEQ) {
        if (!isNumericType(left) || !isNumericType(right)) {
            addError("Comparison operation requires numeric operands");
            return DataType::UNKNOWN;
        }
        return DataType::BOOL;
    }
    
    // Equality operations
    if (op == EQL || op == NEQ) {
        if (!isCompatibleTypes(left, right)) {
            addError("Equality operation requires compatible operands");
            return DataType::UNKNOWN;
        }
        return DataType::BOOL;
    }
    
    // Logical operations
    if (op == AND || op == OR) {
        if (left != DataType::BOOL || right != DataType::BOOL) {
            addError("Logical operation requires boolean operands");
            return DataType::UNKNOWN;
        }
        return DataType::BOOL;
    }
    
    return DataType::UNKNOWN;
}

DataType SemanticAnalyzer::checkUnaryOperation(DataType operand, TokenCodes op, int line) {
    setCurrentLine(line);
    
    if (op == NOT) {
        if (operand != DataType::BOOL) {
            addError("Logical NOT operation requires boolean operand");
            return DataType::UNKNOWN;
        }
        return DataType::BOOL;
    }
    
    if (op == MINUS) {
        if (!isNumericType(operand)) {
            addError("Unary minus operation requires numeric operand");
            return DataType::UNKNOWN;
        }
        return operand;
    }
    
    return DataType::UNKNOWN;
}

void SemanticAnalyzer::addError(const string& message) {
    errors.push_back(formatError(message, currentLine));
}

void SemanticAnalyzer::addWarning(const string& message) {
    warnings.push_back(formatWarning(message, currentLine));
}

bool SemanticAnalyzer::hasErrors() const {
    return !errors.empty();
}

bool SemanticAnalyzer::hasWarnings() const {
    return !warnings.empty();
}

void SemanticAnalyzer::printErrors() const {
    if (hasErrors()) {
        cout << "\n=== Semantic Errors ===" << endl;
        for (const string& error : errors) {
            cout << error << endl;
        }
        cout << "======================" << endl;
    }
}

void SemanticAnalyzer::printWarnings() const {
    if (hasWarnings()) {
        cout << "\n=== Semantic Warnings ===" << endl;
        for (const string& warning : warnings) {
            cout << warning << endl;
        }
        cout << "=========================" << endl;
    }
}

void SemanticAnalyzer::printSemanticReport() const {
    printErrors();
    printWarnings();
    
    cout << "\n=== Semantic Analysis Summary ===" << endl;
    cout << "Errors: " << errors.size() << endl;
    cout << "Warnings: " << warnings.size() << endl;
    cout << "=================================" << endl;
}

void SemanticAnalyzer::setCurrentLine(int line) {
    currentLine = line;
}

void SemanticAnalyzer::printSymbolTable() const {
    symbolTable->printSymbolTable();
}

bool SemanticAnalyzer::isNumericType(DataType type) {
    return type == DataType::INT || type == DataType::FLOAT;
}

bool SemanticAnalyzer::isCompatibleTypes(DataType type1, DataType type2) {
    if (type1 == type2) return true;
    
    // Allow implicit conversions between numeric types
    if (isNumericType(type1) && isNumericType(type2)) return true;
    
    return false;
}

DataType SemanticAnalyzer::getPromotedType(DataType type1, DataType type2) {
    // Float has higher precedence than int
    if (type1 == DataType::FLOAT || type2 == DataType::FLOAT) {
        return DataType::FLOAT;
    }
    
    return DataType::INT;
}

string SemanticAnalyzer::formatError(const string& message, int line) {
    return "Error (line " + to_string(line) + "): " + message;
}

string SemanticAnalyzer::formatWarning(const string& message, int line) {
    return "Warning (line " + to_string(line) + "): " + message;
}
