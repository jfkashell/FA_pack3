#include <stdio.h>
#include <stdlib.h>

#define VECTOR_TYPE int
#include "lab3_task2.h"

VECTOR_TYPE copy_int(VECTOR_TYPE x) {
    return x;
}

void delete_int(VECTOR_TYPE x) {
    (void)x;
}

void print_vector(const Vector *v) {
    if (v->size == 0) {
        printf("Vector is empty (Capacity: %zu)\n", v->capacity);
        return;
    }
    printf("Vector (Size: %zu, Capacity: %zu): [ ", v->size, v->capacity);
    for (size_t i = 0; i < v->size; ++i) {
        printf("%d ", v->data[i]);
    }
    printf("]\n");
}

void run_test_push_and_get() {
    printf("[TEST] Push Back & Get At... ");
    Vector v = create_vector(2, copy_int, delete_int);
    push_back_vector(&v, 10);
    push_back_vector(&v, 20);
    push_back_vector(&v, 30); 
    int val1, val2, val3;
    get_at_vector(&v, 0, &val1);
    get_at_vector(&v, 1, &val2);
    get_at_vector(&v, 2, &val3);
    if (v.size == 3 && val1 == 10 && val2 == 20 && val3 == 30) printf("PASS\n");
    else printf("FAIL\n");
    delete_vector(&v);
}

void run_test_delete_at() {
    printf("[TEST] Delete At... ");
    Vector v = create_vector(5, NULL, NULL);
    push_back_vector(&v, 10);
    push_back_vector(&v, 20);
    push_back_vector(&v, 30);
    delete_at_vector(&v, 1);
    int val1, val2;
    get_at_vector(&v, 0, &val1);
    get_at_vector(&v, 1, &val2);
    if (v.size == 2 && val1 == 10 && val2 == 30) printf("PASS\n");
    else printf("FAIL\n");
    delete_vector(&v);
}

void run_interactive_mode() {
    printf("\n--- Interactive Manual Test ---\n");
    
    Vector v = create_vector(4, copy_int, delete_int);
    int choice;
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Push Back (add element)\n");
        printf("2. Get At (get element by index)\n");
        printf("3. Delete At (remove element by index)\n");
        printf("4. Print Vector status\n");
        printf("5. Clear Vector\n");
        printf("0. Exit\n");
        printf("Select action: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int val;
                printf("Enter integer value: ");
                scanf("%d", &val);
                if (push_back_vector(&v, val) == VECTOR_OK) {
                    printf("Added %d\n", val);
                } else {
                    printf("Error adding element (Memory?)\n");
                }
                break;
            }
            case 2: {
                size_t idx;
                printf("Enter index (0-%zu): ", v.size > 0 ? v.size - 1 : 0);
                scanf("%zu", &idx);
                int result;
                vector_status status = get_at_vector(&v, idx, &result);
                if (status == VECTOR_OK) {
                    printf("Value at [%zu] is: %d\n", idx, result);
                } else {
                    printf("Error: Index out of bounds\n");
                }
                break;
            }
            case 3: {
                size_t idx;
                printf("Enter index to delete: ");
                scanf("%zu", &idx);
                if (delete_at_vector(&v, idx) == VECTOR_OK) {
                    printf("Element deleted.\n");
                } else {
                    printf("Error: Index out of bounds\n");
                }
                break;
            }
            case 4:
                print_vector(&v);
                break;
            case 5:
                erase_vector(&v);
                printf("Vector cleared (Capacity preserved).\n");
                break;
            default:
                printf("Unknown command.\n");
        }
    }

    delete_vector(&v);
    printf("Memory freed. Exiting.\n");
}

int main() {
    printf("=== Running Auto Tests ===\n");
    run_test_push_and_get();
    run_test_delete_at();
    printf("=== Auto Tests Finished ===\n");
    run_interactive_mode();

    return 0;
}