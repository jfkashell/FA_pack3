#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3_task1.h"

void run_test_case(int test_id, unsigned int num, int r, const char* expected_str, status_code expected_code) {
    char* result_str = NULL;
    status_code code = convert_to_base_2r(num, r, &result_str);

    if (code != expected_code) {
        printf("[FAIL] Test %d: Expected status %d, got %d\n", test_id, expected_code, code);
        if (result_str) free(result_str);
        return;
    }

    if (expected_code != OK) {
        printf("[PASS] Test %d: Correctly handled error input.\n", test_id);
        return;
    }

    if (strcmp(result_str, expected_str) == 0) {
        printf("[PASS] Test %d: %u (base 2^%d) -> %s\n", test_id, num, r, result_str);
    } else {
        printf("[FAIL] Test %d: Input %u, r=%d. Expected '%s', got '%s'\n", 
               test_id, num, r, expected_str, result_str);
    }

    free(result_str);
}

void run_all_tests() {
    printf("--- Running Tests ---\n");
    
    run_test_case(1, 10, 1, "1010", OK);

    run_test_case(2, 10, 2, "22", OK);

    run_test_case(3, 255, 3, "377", OK);

    run_test_case(4, 255, 4, "FF", OK);

    run_test_case(5, 33, 5, "11", OK);

    run_test_case(6, 0, 3, "0", OK);

    run_test_case(7, 31, 5, "V", OK);

    run_test_case(8, 100, 6, NULL, INVALID_INPUT);

    printf("--- Tests Finished ---\n\n");
}

int main() {
    run_all_tests();

    printf("--- Interactive Mode ---\n");
    unsigned int num;
    int r;
    char* result_str = NULL;

    printf("Enter number: ");
    if (scanf("%u", &num) != 1) return 1;

    printf("Enter r (1-5): ");
    if (scanf("%d", &r) != 1) return 1;

    status_code code = convert_to_base_2r(num, r, &result_str);

    if (code == OK) {
        printf("Result: %s\n", result_str);
        free(result_str);
    } else if (code == INVALID_INPUT) {
        fprintf(stderr, "Error: Invalid r (must be 1-5)\n");
    } else {
        fprintf(stderr, "Error allocation memory\n");
    }

    return 0;
}