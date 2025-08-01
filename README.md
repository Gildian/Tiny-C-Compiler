# Tiny C Compiler

A C-like compiler that supports lexical analysis, syntax analysis, and semantic analysis with comprehensive error reporting.

## Features

### Core Language Support
- **Data Types**: `int`, `float`, `bool`, `void`
- **Control Structures**: `if/else`, `while`, `for`, `do-while`
- **Functions**: Function declarations with parameters and return types
- **Variables**: Variable declarations and assignments
- **Expressions**: Full expression evaluation with operator precedence
- **Built-in Functions**: `printf`, `scanf`

### Array Support (NEW!)
- **Array Declarations**: Support for one-dimensional arrays with fixed sizes
  ```c
  int numbers[10];
  float values[5];
  bool flags[3];
  ```
- **Mixed Declarations**: Arrays and variables in the same declaration
  ```c
  int x, arr[10], y, matrix[5];
  ```
- **Array Access**: Array indexing with expressions
  ```c
  arr[0] = 10;
  arr[i] = arr[0] + 5;
  x = arr[index];
  ```
- **Semantic Checking**: 
  - Undeclared array detection
  - Array vs. variable type checking
  - Array size validation (must be positive)

### Error Handling
- **Syntax Errors**: Detailed error messages with line numbers and expected tokens
- **Semantic Errors**: Type checking, undeclared variable/array detection
- **Warning System**: Implicit type conversions and other potential issues

## Building

```bash
make clean
make
```

## Testing

### Run All Tests
```bash
make test
```

### Run Specific Test Categories
```bash
make test-basic      # Basic syntax tests
make test-semantic   # Semantic analysis tests
make test-arrays     # Array functionality tests
```

### Run Individual Tests
```bash
./compiler tests/test_arrays.c
./compiler tests/test_mixed_arrays.c
./compiler tests/test_array_errors.c
```

## Array Examples

### Basic Array Usage
```c
int main()
{
    int arr[5];
    int x, y;
    
    x = 10;
    arr[0] = x;
    arr[1] = 20;
    y = arr[0];
    
    return 0;
}
```

### Mixed Arrays and Variables
```c
int main()
{
    int x, arr[10], y, matrix[3];
    float farr[5];
    bool flags[2];
    
    x = 42;
    arr[0] = x;
    farr[0] = 3.14;
    flags[0] = true;
    
    return 0;
}
```

### Array Error Detection
```c
int main()
{
    int arr[5];
    int x;
    
    arr[0] = 10;        // Valid
    x[0] = 5;           // Error: 'x' is not an array
    undeclared[0] = 20; // Error: Undeclared array
    
    return 0;
}
```

## Architecture

- **Token/TokenCodes**: Lexical token representation with support for `[` and `]`
- **LexicalAnalyzer**: Tokenization with array bracket recognition
- **SyntaxAnalyzer**: Recursive descent parser with array grammar support
- **SemanticAnalyzer**: Type checking and scope management for arrays
- **SymbolTable**: Multi-scope symbol management with array metadata

## Grammar Extensions

The array feature extends the original grammar with:

```
<array-declaration> → <type> <identifier> [ <number> ]
<array-access> → <identifier> [ <expression> ]
<primary> → <array-access> | <identifier> | <number> | ( <expression> )
```

## Future Enhancements

- Multi-dimensional arrays
- Array parameters in functions
- Dynamic array allocation
- Array initialization syntax
- String arrays and string literals
