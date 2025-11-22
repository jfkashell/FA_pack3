#include <stdio.h>
#include <stdlib.h>
#include "brackets.h"

void run_test(int id, const char *input, int expected) {
    int result = check_brackets(input);
    if (result == expected) {
        printf("[PASS] Test %d: Input '%s'\n", id, input);
    } else {
        printf("[FAIL] Test %d: Input '%s'. Expected %d, got %d\n", id, input, expected, result);
    }
}

int main() {
    printf("--- Running Tests ---\n");
    
    run_test(1, "()", 1);
    run_test(2, "([])", 1);
    run_test(3, "{a + [b] * (c)}", 1); // С текстом [cite: 226]
    run_test(4, "<{()}>", 1); // Все типы скобок [cite: 228]
    
    run_test(5, "(", 0);          
    run_test(6, ")", 0);          
    run_test(7, "(]", 0);         
    run_test(8, "([)]", 0);       
    run_test(9, "><", 0);         
    run_test(10, "((((", 0);
    
    run_test(11, "", 1); 
    run_test(12, "abc 123", 1);

    printf("--- Tests Finished ---\n\n");

    char buffer[256];
    printf("Enter a string to check brackets: ");
    
    if (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = 0;
        while(buffer[len]) len++;
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

        int res = check_brackets(buffer);
        if (res == 1) {
            printf("Result: Correct (Balanced)\n");
        } else if (res == 0) {
            printf("Result: Incorrect (Unbalanced)\n");
        } else {
            printf("Result: Memory Error\n");
        }
    }

    return 0;
}