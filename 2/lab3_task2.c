#include <stdio.h>
#include <string.h> 
#ifndef VECTOR_TYPE
#define VECTOR_TYPE int
#endif

#include "lab3_task2.h"

Vector create_vector(size_t initial_capacity, 
                     VECTOR_TYPE (*CopyFunc)(VECTOR_TYPE), 
                     void (*DeleteFunc)(VECTOR_TYPE)) {
    Vector v;
    v.size = 0;
    v.capacity = initial_capacity > 0 ? initial_capacity : 4;
    v.CopyFunc = CopyFunc;
    v.DeleteFunc = DeleteFunc;
    
    v.data = (VECTOR_TYPE*)malloc(v.capacity * sizeof(VECTOR_TYPE));
    
    if (v.data == NULL) {
        v.capacity = 0;
    }
    
    return v;
}

vector_status reserve_vector(Vector *v, size_t new_capacity) {
    if (new_capacity <= v->capacity) return VECTOR_OK;

    VECTOR_TYPE *new_data = (VECTOR_TYPE*)realloc(v->data, new_capacity * sizeof(VECTOR_TYPE));
    if (new_data == NULL) {
        return VECTOR_MEMORY_ERROR;
    }
    
    v->data = new_data;
    v->capacity = new_capacity;
    return VECTOR_OK;
}

vector_status push_back_vector(Vector *v, VECTOR_TYPE value) {
    if (v->data == NULL && v->capacity == 0) {
         v->capacity = 4;
         v->data = (VECTOR_TYPE*)malloc(v->capacity * sizeof(VECTOR_TYPE));
         if (!v->data) return VECTOR_MEMORY_ERROR;
    }

    if (v->size >= v->capacity) {
        if (reserve_vector(v, v->capacity * 2) != VECTOR_OK) {
            return VECTOR_MEMORY_ERROR;
        }
    }

    if (v->CopyFunc) {
        v->data[v->size] = v->CopyFunc(value);
    } else {
        v->data[v->size] = value;
    }
    
    v->size++;
    return VECTOR_OK;
}

vector_status get_at_vector(const Vector *v, size_t index, VECTOR_TYPE *result) {
    if (index >= v->size) return VECTOR_INDEX_ERROR;
    *result = v->data[index];
    return VECTOR_OK;
}

vector_status delete_at_vector(Vector *v, size_t index) {
    if (index >= v->size) return VECTOR_INDEX_ERROR;

    if (v->DeleteFunc) {
        v->DeleteFunc(v->data[index]);
    }

    for (size_t i = index; i < v->size - 1; ++i) {
        v->data[i] = v->data[i + 1];
    }

    v->size--;
    return VECTOR_OK;
}

void erase_vector(Vector *v) {
    if (v->DeleteFunc) {
        for (size_t i = 0; i < v->size; ++i) {
            v->DeleteFunc(v->data[i]);
        }
    }
    v->size = 0;
}

void delete_vector(Vector *v) {
    erase_vector(v); 
    free(v->data);   
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
}

int is_equal_vector(const Vector *v1, const Vector *v2) {
    if (v1->size != v2->size) return 0;

    for (size_t i = 0; i < v1->size; ++i) {
        if (v1->data[i] != v2->data[i]) return 0;
    }
    return 1;
}

vector_status copy_vector(Vector *dest, const Vector *src) {
    erase_vector(dest);

    if (reserve_vector(dest, src->size) != VECTOR_OK) {
        return VECTOR_MEMORY_ERROR;
    }

    for (size_t i = 0; i < src->size; ++i) {
        if (dest->CopyFunc) {
             dest->data[i] = dest->CopyFunc(src->data[i]);
        } else {
             dest->data[i] = src->data[i];
        }
    }
    dest->size = src->size;
    return VECTOR_OK;
}