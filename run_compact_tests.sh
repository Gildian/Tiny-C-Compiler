#!/bin/bash

# Compact Test Suite for Tiny C Compiler
# This script runs all test cases and provides a summary

echo "=============================================="
echo "Tiny C Compiler - Compact Test Suite"
echo "=============================================="

cd "$(dirname "$0")"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
total_tests=0
passed_tests=0
failed_tests=0

# Function to run a test
run_test() {
    local test_file=$1
    local expected_result=$2  # "pass" or "fail"
    local test_name=$3
    
    echo -n "Testing $test_name... "
    
    if ./compiler "$test_file" > /dev/null 2>&1; then
        actual_result="pass"
    else
        actual_result="fail"
    fi
    
    total_tests=$((total_tests + 1))
    
    if [ "$actual_result" = "$expected_result" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "${RED}✗ FAIL${NC} (expected $expected_result, got $actual_result)"
        failed_tests=$((failed_tests + 1))
    fi
}

# Check if compiler exists
if [ ! -f "./compiler" ]; then
    echo -e "${RED}Error: Compiler not found. Please run 'make' first.${NC}"
    exit 1
fi

echo "Running tests..."
echo ""

# Valid programs (should pass)
echo -e "${YELLOW}Valid Programs:${NC}"
run_test "tests/valid_programs.c" "pass" "Comprehensive valid program"

# Syntax errors (should fail)
echo -e "${YELLOW}Syntax Errors:${NC}"
run_test "tests/syntax_errors.c" "fail" "Syntax error detection"

# Semantic errors (should fail)
echo -e "${YELLOW}Semantic Errors:${NC}"
run_test "tests/semantic_errors.c" "fail" "Semantic error detection"

# Legacy tests (should pass)
echo -e "${YELLOW}Legacy Tests:${NC}"
run_test "tests/test1.c" "pass" "Basic test (legacy)"

# Summary
echo ""
echo "=============================================="
echo "Test Summary:"
echo "Total tests: $total_tests"
echo -e "Passed: ${GREEN}$passed_tests${NC}"
echo -e "Failed: ${RED}$failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
