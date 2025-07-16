# Tiny C Compiler

A simple compiler for a subset of the C programming language, implemented in C++. This compiler consists of a lexical analyzer (tokenizer) and a syntax analyzer (parser) that can parse and validate Tiny C programs.

## Features

- **Lexical Analysis**: Tokenizes Tiny C source code into meaningful tokens
- **Syntax Analysis**: Parses tokens according to Tiny C grammar rules
- **Error Reporting**: Provides meaningful error messages with position indicators
- **Support for**:
  - Variable declarations (int, float, bool, void)
  - Function definitions
  - Control structures (if/else, while, for, do-while)
  - Expressions and operators
  - Assignment statements
  - I/O functions (printf, scanf)

## Grammar Support

The compiler supports a subset of C including:

- **Data Types**: `int`, `float`, `bool`, `void`
- **Keywords**: `if`, `else`, `while`, `for`, `do`, `return`, `true`, `false`
- **Operators**: `+`, `-`, `*`, `/`, `%`, `=`, `==`, `!=`, `<`, `<=`, `>`, `>=`, `&&`, `||`, `!`
- **Delimiters**: `(`, `)`, `{`, `}`, `,`, `;`
- **Identifiers**: Variable and function names
- **Literals**: Integer and floating-point numbers, boolean values

## Building the Compiler

### Prerequisites
- GCC or compatible C++ compiler with C++11 support
- Make (optional, for using Makefile)

### Compilation
```bash
# Using the provided build task
g++-15 -std=c++11 -Wall -g -o compiler main.cpp LexicalAnalyzer.cpp SyntaxAnalyzer.cpp Token.cpp

# Or using make (if Makefile is configured)
make
```

## Usage

```bash
./compiler <source_file>
```

### Example
```bash
./compiler test1.c
./compiler test2.c
./compiler test3.c
```

## Test Cases

The repository includes several test cases:

- **test1.c**: Basic variable declaration
- **test2.c**: Variable declaration with initialization and comparison
- **test3.c**: Control structures with if statements and function calls

## Files Description

- `main.cpp` - Main driver program
- `LexicalAnalyzer.h/cpp` - Lexical analyzer implementation
- `SyntaxAnalyzer.h/cpp` - Syntax analyzer implementation  
- `Token.h/cpp` - Token class implementation
- `TokenCodes.h` - Token type definitions
- `test1.c`, `test2.c` - Test input files
- `Makefile` - Build configuration

## Improvements Made

### 1. Fixed Compilation Issues
- Added missing `#include <iostream>` to `SyntaxAnalyzer.cpp`
- Added missing `#include <string>` to `LexicalAnalyzer.h`
- Fixed function call syntax in `Function()` method

### 2. Fixed Logic Errors in SyntaxAnalyzer
- **Fixed boolean logic**: Changed `&&` to `||` in multiple `*Prime()` methods:
  - `EqualityPrime()`: Fixed `(EQL && NEQ)` → `(EQL || NEQ)`  
  - `RelationalPrime()`: Fixed `(LSS && LEQ && GTR && GEQ)` → `(LSS || LEQ || GTR || GEQ)`
  - `TermPrime()`: Fixed `(PLUS && MINUS)` → `(PLUS || MINUS)`
  - `FactorPrime()`: Fixed `(TIMES && SLASH && MOD)` → `(TIMES || SLASH || MOD)`

### 3. Fixed Parser State Management
- **Added missing token advancement** in multiple statement cases
- **Fixed token advancement** in `Primary()`, `Compstmt()`, and other methods
- **Fixed recursive calls** in `*Prime()` methods for proper left-recursion elimination

### 4. Fixed Statement Parsing
- **FOR loop**: Added proper token advancement between expressions
- **IF statement**: Fixed error token in condition check (`LPAREN` vs `RPAREN`)
- **RETURN statement**: Added missing token advancement before expression
- **WHILE statement**: Added missing token advancement before expression
- **PRINTF/SCANF**: Added missing token advancement after statements

### 5. Fixed Expression and Primary Parsing
- **Primary()**: Complete rewrite with proper logic flow and token advancement
- **Expression()**: Improved assignment detection and handling
- **Unary()**: Fixed duplicate `Primary()` call

### 6. Fixed Lexical Analyzer
- **Added missing tokens**: `ASSIGN` (`=`), `NOT` (`!`), `MOD` (`%`)
- **Improved token recognition** for assignment operations

### 7. Fixed Test Files
- **test2.c**: Fixed syntax error (missing comma between `y` and `z`)

### 8. Improved Main Program
- **Added command-line argument validation**
- **Added exception handling** for parser errors
- **Added proper memory cleanup** with delete statements
- **Added proper file closing**
- **Improved error messages** with filename information

### 9. Added Build Support
- **Created Makefile** with proper dependencies and build targets
- **Added clean and test targets** for easy development

### 10. Code Quality Improvements
- **Better error handling** throughout the codebase
- **Consistent code formatting** and structure
- **Added meaningful comments** for complex logic
- **Memory management** improvements

## Usage

### Building the Project
```bash
make
```

### Running the Compiler
```bash
./compiler test1.c
./compiler test2.c
```

### Testing
```bash
make test
```

### Cleaning Build Files
```bash
make clean
```

## Grammar Support

The compiler supports parsing of:
- Function declarations with parameters
- Variable declarations
- Control flow statements (if/else, while, for, do-while)
- Expressions with proper operator precedence
- Boolean and arithmetic operations
- Return statements
- Printf/scanf statements

## Architecture

1. **Lexical Analyzer**: Tokenizes the input source code
2. **Syntax Analyzer**: Parses tokens according to the grammar rules
3. **Token System**: Represents different types of language constructs
4. **Error Handling**: Provides meaningful error messages with position information

## Error Recovery

The parser includes error reporting with:
- Position information showing where the error occurred
- Expected token information
- Graceful program termination on syntax errors
