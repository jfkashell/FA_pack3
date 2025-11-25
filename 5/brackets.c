#include "brackets.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t top;
    size_t capacity;
} CharStack;

int init_stack(CharStack *s) {
    s->capacity = 16;
    s->top = 0;
    s->data = (char*)malloc(s->capacity * sizeof(char));
    return (s->data != NULL);
}

int push(CharStack *s, char c) {
    if (s->top >= s->capacity) {
        s->capacity *= 2;
        char *new_data = (char*)realloc(s->data, s->capacity * sizeof(char));
        if (!new_data) return 0;
        s->data = new_data;
    }
    s->data[s->top++] = c;
    return 1;
}

char pop(CharStack *s) {
    if (s->top == 0) return 0;
    return s->data[--s->top];
}

int is_matching_pair(char open, char close) {
    if (open == '(' && close == ')') return 1;
    if (open == '[' && close == ']') return 1;
    if (open == '{' && close == '}') return 1;
    if (open == '<' && close == '>') return 1;
    return 0;
}

int check_brackets(const char *str) {
    if (str == NULL) return 0;

    CharStack stack;
    if (!init_stack(&stack)) return -1;

    int result = 1;
    int any_brackets = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        if (current == '(' || current == '[' || current == '{' || current == '<' ||
            current == ')' || current == ']' || current == '}' || current == '>') {
            
            any_brackets = 1;

            if (current == '(' || current == '[' || current == '{' || current == '<') {
                if (!push(&stack, current)) {
                    free(stack.data);
                    return -1;
                }
            }
            else {
                if (stack.top == 0) {
                    result = 0;
                    break;
                }
                char open = pop(&stack);
                if (!is_matching_pair(open, current)) {
                    result = 0;
                    break;
                }
            }
        }
    }

    if (stack.top > 0) {
        result = 0;
    }

    if (any_brackets == 0) {
        result = 0;
    }

    free(stack.data);
    return result;
}