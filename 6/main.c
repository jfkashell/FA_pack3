#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

int main(int argc, char *argv[]) {
    const char *input_file = "input.txt";
    const char *trace_file = "trace.log";

    if (argc > 1) input_file = argv[1];

    FILE *in = fopen(input_file, "r");
    if (!in) {
        printf("Input file not found. Creating default 'input.txt'...\n");
        in = fopen("input.txt", "w");
        fprintf(in, "A=2\n");
        fprintf(in, "B=3\n");
        fprintf(in, "C=A+B*2\n");
        fprintf(in, "D=C^B\n");
        fprintf(in, "print(D)\n");
        fclose(in);
        in = fopen("input.txt", "r");
    }

    FILE *log = fopen(trace_file, "w");
    if (!log) {
        perror("Cannot open log file");
        fclose(in);
        return 1;
    }

    Context ctx;
    init_context(&ctx);

    printf("Processing '%s' -> Trace to '%s'...\n", input_file, trace_file);
    printf("--- Console Output ---\n");

    char line[256];
    while (fgets(line, sizeof(line), in)) {
        process_command(line, &ctx, log);
    }

    printf("--- Done ---\n");

    fclose(in);
    fclose(log);
    return 0;
}