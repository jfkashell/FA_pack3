#ifndef LAB4_H
#define LAB4_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum {
    STATUS_PROCESSING,
    STATUS_DELIVERED,
    STATUS_UNDELIVERED,
    STATUS_IN_TRANSIT
} LetterStatus;

typedef struct {
    unsigned long id; 
    char type[32];   
    int priority; 
    int sender_id;
    int receiver_id;
    char tech_data[100];
    time_t created_at;
    LetterStatus status;
} Letter;

#define HEAP_TYPE Letter

typedef struct {
    HEAP_TYPE *data;
    size_t size;
    size_t capacity;
} Heap;

Heap create_heap(size_t initial_capacity);

void delete_heap(Heap *h);

void push_heap(Heap *h, HEAP_TYPE value);

HEAP_TYPE pop_heap(Heap *h);

HEAP_TYPE peek_heap(const Heap *h);

int is_empty_heap(const Heap *h);
size_t size_heap(const Heap *h);

typedef struct {
    int id;
    size_t max_capacity;
    Heap mail_storage;
    
    int *connected_ids;
    size_t connections_count;
} PostOffice;

#endif