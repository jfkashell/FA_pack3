#include "linked_list.h"
#include <stdio.h>

LinkedList create_list(void) {
    LinkedList list;
    list.head = NULL;
    list.tail = NULL;
    list.size = 0;
    return list;
}

void push_back_list(LinkedList *list, LIST_TYPE value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return; 

    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail) {
        list->tail->next = new_node;
    }
    list->tail = new_node;

    if (!list->head) { 
        list->head = new_node;
    }
    list->size++;
}

void push_front_list(LinkedList *list, LIST_TYPE value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return;

    new_node->data = value;
    new_node->prev = NULL;
    new_node->next = list->head;

    if (list->head) {
        list->head->prev = new_node;
    }
    list->head = new_node;

    if (!list->tail) {
        list->tail = new_node;
    }
    list->size++;
}

void insert_at_list(LinkedList *list, size_t index, LIST_TYPE value) {
    if (index == 0) {
        push_front_list(list, value);
        return;
    }
    if (index >= list->size) {
        push_back_list(list, value);
        return;
    }

    Node *current = list->head;
    for (size_t i = 0; i < index; i++) current = current->next;

    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return;

    new_node->data = value;
    new_node->prev = current->prev;
    new_node->next = current;

    current->prev->next = new_node;
    current->prev = new_node;
    list->size++;
}

void delete_at_list(LinkedList *list, size_t index) {
    if (index >= list->size) return;

    Node *to_delete = list->head;
    for (size_t i = 0; i < index; i++) to_delete = to_delete->next;

    if (to_delete->prev) to_delete->prev->next = to_delete->next;
    else list->head = to_delete->next;

    if (to_delete->next) to_delete->next->prev = to_delete->prev;
    else list->tail = to_delete->prev;

    free(to_delete);
    list->size--;
}

LIST_TYPE get_at_list(const LinkedList *list, size_t index) {
    Node *current = list->head;
    for (size_t i = 0; i < index && current; i++) current = current->next;    

    return current ? current->data : (LIST_TYPE){0}; 
}

void erase_list(LinkedList *list) {
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void delete_list(LinkedList *list) {
    erase_list(list);
}

void push_stack(LinkedList *stack, LIST_TYPE value) { push_back_list(stack, value); }
LIST_TYPE pop_stack(LinkedList *stack) { 
    LIST_TYPE val = get_at_list(stack, stack->size - 1);
    delete_at_list(stack, stack->size - 1);
    return val;
}

void enqueue(LinkedList *queue, LIST_TYPE value) { push_back_list(queue, value); }
LIST_TYPE dequeue(LinkedList *queue) {
    LIST_TYPE val = get_at_list(queue, 0);
    delete_at_list(queue, 0);
    return val;
}