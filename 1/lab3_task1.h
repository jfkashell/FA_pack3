#ifndef LAB3_TASK1_H
#define LAB3_TASK1_H

#include <stdlib.h>

typedef enum {
    OK = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2
} status_code;

status_code convert_to_base_2r(unsigned int number, int r, char** result);

#endif