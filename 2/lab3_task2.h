#ifndef LAB3_TASK2_H
#define LAB3_TASK2_H

#include <stdlib.h>
#include <stdbool.h>

#ifndef VECTOR_TYPE
#error "VECTOR_TYPE must be defined before including this header"
#endif

typedef enum {
    VECTOR_OK = 0,
    VECTOR_MEMORY_ERROR = 1,
    VECTOR_INDEX_ERROR = 2,
    VECTOR_INVALID_INPUT = 3
} vector_status;

typedef struct {
    VECTOR_TYPE *data;     
    size_t size;           
    size_t capacity;     
    
    VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE); 
    void (*DeleteFunc)(VECTOR_TYPE);      
} Vector;

Vector create_vector(size_t initial_capacity, 
                     VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), 
                     void (*DeleteFunc)(VECTOR_TYPE));

vector_status push_back_vector(Vector *v, VECTOR_TYPE value);

vector_status delete_at_vector(Vector *v, size_t index);

vector_status get_at_vector(const Vector *v, size_t index, VECTOR_TYPE *result);

void erase_vector(Vector *v);

void delete_vector(Vector *v);

int is_equal_vector(const Vector *v1, const Vector *v2);

vector_status copy_vector(Vector *dest, const Vector *src);

vector_status reserve_vector(Vector *v, size_t new_capacity);

#endif