# Test Suite Documentation

## Overview
This directory contains a compact test suite for the Tiny C Compiler with comprehensive test coverage.

## Test Files

### 1. valid_programs.c
Contains multiple valid C programs that should compile successfully:
- Simple variable declarations
- Variable assignments
- Functions with parameters
- Control flow structures (if/else)
- Loops (for, while, do-while)
- Different data types (int, float, bool)
- Expressions and operators

### 2. syntax_errors.c
Contains programs with syntax errors that should fail compilation:
- Missing semicolons
- Missing parentheses
- Missing braces
- Invalid token sequences
- Missing commas in parameter lists
- Missing return types

### 3. semantic_errors.c
Contains programs with semantic errors that should fail compilation:
- Undeclared variables
- Variable redeclaration
- Function redefinition
- Void variable declarations
- Type mismatches
- Using variables before declaration
- Scope-related issues

### 4. test1.c & test2.c
Legacy test files maintained for backward compatibility.

## Running Tests

### Quick Test
```bash
./run_compact_tests.sh
```

### Manual Testing
```bash
# Test a valid program (should succeed)
./compiler tests/valid_programs.c

# Test syntax errors (should fail)
./compiler tests/syntax_errors.c

# Test semantic errors (should fail)
./compiler tests/semantic_errors.c
```

## Test Coverage
The compact test suite covers:
- ✅ Lexical analysis
- ✅ Syntax parsing
- ✅ Semantic analysis
- ✅ Symbol table management
- ✅ Scope handling
- ✅ Type checking
- ✅ Error handling and reporting

## Benefits of Compact Design
1. **Easier Maintenance**: Fewer files to manage
2. **Better Organization**: Related test cases grouped together
3. **Faster Execution**: Single file compilation vs. multiple files
4. **Clear Coverage**: Each file has a specific purpose
5. **Simplified CI/CD**: Fewer test files to track
