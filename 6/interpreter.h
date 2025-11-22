#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_VARS 26

typedef struct {
    int values[MAX_VARS];
    bool is_set[MAX_VARS];
} Context;

void init_context(Context *ctx);

int process_command(const char *line, Context *ctx, FILE *log_file);

int fast_pow(int base, int exp);

#endif