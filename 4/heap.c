#include "lab4.h"

size_t parent(size_t i) { return (i - 1) / 2; }
size_t left_child(size_t i) { return 2 * i + 1; }
size_t right_child(size_t i) { return 2 * i + 2; }

void swap(HEAP_TYPE *a, HEAP_TYPE *b) {
    HEAP_TYPE temp = *a;
    *a = *b;
    *b = temp;
}

int compare_letters(HEAP_TYPE a, HEAP_TYPE b) {
    return a.priority > b.priority;
}

Heap create_heap(size_t initial_capacity) {
    Heap h;
    h.size = 0;
    h.capacity = initial_capacity > 0 ? initial_capacity : 4;
    h.data = (HEAP_TYPE*)malloc(h.capacity * sizeof(HEAP_TYPE));
    return h;
}

void delete_heap(Heap *h) {
    if (h->data) {
        free(h->data);
        h->data = NULL;
    }
    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h) {
    return h->size == 0;
}

size_t size_heap(const Heap *h) {
    return h->size;
}

HEAP_TYPE peek_heap(const Heap *h) {
    if (is_empty_heap(h)) {
        HEAP_TYPE empty = {0};
        return empty;
    }
    return h->data[0];
}

void sift_up(Heap *h, size_t index) {
    while (index > 0 && compare_letters(h->data[index], h->data[parent(index)])) {
        swap(&h->data[index], &h->data[parent(index)]);
        index = parent(index);
    }
}

void sift_down(Heap *h, size_t index) {
    size_t max_index = index;
    size_t l = left_child(index);
    size_t r = right_child(index);

    if (l < h->size && compare_letters(h->data[l], h->data[max_index])) {
        max_index = l;
    }
    if (r < h->size && compare_letters(h->data[r], h->data[max_index])) {
        max_index = r;
    }

    if (index != max_index) {
        swap(&h->data[index], &h->data[max_index]);
        sift_down(h, max_index);
    }
}

void push_heap(Heap *h, HEAP_TYPE value) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->data = (HEAP_TYPE*)realloc(h->data, h->capacity * sizeof(HEAP_TYPE));
    }
    h->data[h->size] = value;
    sift_up(h, h->size);
    h->size++;
}

HEAP_TYPE pop_heap(Heap *h) {
    if (is_empty_heap(h)) {
        HEAP_TYPE empty = {0};
        return empty;
    }

    HEAP_TYPE result = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    sift_down(h, 0);
    
    return result;
}