#include "interpreter.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define STACK_SIZE 100

typedef struct {
    int data[STACK_SIZE];
    int top;
} IntStack;

typedef struct {
    char data[STACK_SIZE];
    int top;
} CharStack;

void push_int(IntStack *s, int v) { s->data[s->top++] = v; }
int pop_int(IntStack *s) { return s->data[--s->top]; }
int peek_int(IntStack *s) { return s->data[s->top - 1]; }

void push_char(CharStack *s, char v) { s->data[s->top++] = v; }
char pop_char(CharStack *s) { return s->data[--s->top]; }
char peek_char(CharStack *s) { return s->top > 0 ? s->data[s->top - 1] : 0; }

void init_context(Context *ctx) {
    for(int i=0; i<MAX_VARS; i++) {
        ctx->values[i] = 0;
        ctx->is_set[i] = false;
    }
}

int fast_pow(int base, int exp) {
    int res = 1;
    while (exp > 0) {
        if (exp % 2 == 1) res *= base;
        base *= base;
        exp /= 2;
    }
    return res;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

int apply_op(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0; 
        case '^': return fast_pow(a, b);
    }
    return 0;
}

int evaluate_expression(const char *expr, Context *ctx, int *error) {
    IntStack values = { .top = 0 };
    CharStack ops = { .top = 0 };
    *error = 0;

    for (int i = 0; expr[i]; i++) {
        if (isspace(expr[i])) continue;

        if (isdigit(expr[i])) {
            int val = 0;
            while (i < strlen(expr) && isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            push_int(&values, val);
            i--; 
        }
        else if (isupper(expr[i])) {
            int idx = expr[i] - 'A';
            if (!ctx->is_set[idx]) {
                fprintf(stderr, "Error: Variable %c not initialized\n", expr[i]);
                *error = 1;
                return 0;
            }
            push_int(&values, ctx->values[idx]);
        }
        else if (expr[i] == '(') {
            push_char(&ops, '(');
        }
        else if (expr[i] == ')') {
            while (ops.top > 0 && peek_char(&ops) != '(') {
                int val2 = pop_int(&values);
                int val1 = pop_int(&values);
                char op = pop_char(&ops);
                push_int(&values, apply_op(val1, val2, op));
            }
            pop_char(&ops); 
        }
        else {
            while (ops.top > 0 && precedence(peek_char(&ops)) >= precedence(expr[i])) {
                int val2 = pop_int(&values);
                int val1 = pop_int(&values);
                char op = pop_char(&ops);
                push_int(&values, apply_op(val1, val2, op));
            }
            push_char(&ops, expr[i]);
        }
    }

    while (ops.top > 0) {
        int val2 = pop_int(&values);
        int val1 = pop_int(&values);
        char op = pop_char(&ops);
        push_int(&values, apply_op(val1, val2, op));
    }

    if (values.top == 0) return 0;
    return pop_int(&values);
}

void log_trace(FILE *f, const char *cmd, Context *ctx, const char *desc) {
    if (!f) return;
    
    fprintf(f, "[CMD] %s\n", cmd); 
    
    fprintf(f, "| ");
    int first = 1;
    for (int i = 0; i < MAX_VARS; i++) {
        if (ctx->is_set[i]) {
            if (!first) fprintf(f, ", ");
            fprintf(f, "%c=%d", 'A' + i, ctx->values[i]);
            first = 0;
        }
    }
    if (first) fprintf(f, "No vars");
    
    fprintf(f, " | %s\n", desc);
    fprintf(f, "\n");
}

int process_command(const char *line, Context *ctx, FILE *log_file) {
    char buffer[256];
    strcpy(buffer, line);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) == 0) return 1;

    if (strncmp(buffer, "print", 5) == 0) {
        char *start = strchr(buffer, '(');
        char *end = strchr(buffer, ')');
        if (start && end && end > start) {
            char var_name = 0;
            for (char *p = start + 1; p < end; p++) {
                if (isupper(*p)) { var_name = *p; break; }
            }
            
            if (var_name) {
                int idx = var_name - 'A';
                if (ctx->is_set[idx]) {
                    printf("%d\n", ctx->values[idx]); 
                    
                    char desc[64];
                    sprintf(desc, "Print %c", var_name);
                    log_trace(log_file, buffer, ctx, desc);
                } else {
                    printf("Error: %c is undefined\n", var_name);
                }
            }
        }
        return 1;
    }

    char *eq_sign = strchr(buffer, '=');
    if (eq_sign) {
        char target_var = 0;
        for (char *p = buffer; p < eq_sign; p++) {
            if (isupper(*p)) { target_var = *p; break; }
        }

        if (!target_var) return 0; 

        int error = 0;
        int result = evaluate_expression(eq_sign + 1, ctx, &error);
        
        if (!error) {
            int idx = target_var - 'A';
            ctx->values[idx] = result;
            ctx->is_set[idx] = true;

            const char *desc = "Assignment";
            if (strpbrk(eq_sign + 1, "+-*/^")) {
                desc = "Arithmetic operation";
            }
            
            log_trace(log_file, buffer, ctx, desc);
        }
        return 1;
    }

    return 0; 
}