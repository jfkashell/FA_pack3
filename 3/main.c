#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"
#include "liver.h" 

int is_valid_latin(const char* str) {
    if (!str) return 0;
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

int validate_liver(const Liver* l) {
    if (!l) return 0;
    if (strlen(l->surname) == 0 || !is_valid_latin(l->surname)) return 0;
    if (strlen(l->name) == 0 || !is_valid_latin(l->name)) return 0;
    if (strlen(l->patronymic) > 0 && !is_valid_latin(l->patronymic)) return 0;
    if (l->gender != 'M' && l->gender != 'W') return 0;
    if (l->income < 0) return 0;
    if (l->b_day < 1 || l->b_day > 31 || l->b_mon < 1 || l->b_mon > 12 || l->b_year < 1900 || l->b_year > 2025) return 0;
    return 1;
}

void print_liver(const Liver* l) {
    printf("%s %s %s [%02d.%02d.%04d] (%c) Income: %.2f\n",
           l->surname, l->name, l->patronymic,
           l->b_day, l->b_mon, l->b_year,
           l->gender, l->income);
}

typedef enum { CMD_ADD, CMD_DELETE } CmdType;

typedef struct {
    CmdType type;
    Liver data;
    size_t index; 
} Action;

typedef struct {
    Action *actions;
    size_t size;
    size_t capacity;
} UndoStack;

void push_undo(UndoStack *stack, Action act) {
    if (stack->size >= stack->capacity) {
        stack->capacity *= 2;
        stack->actions = realloc(stack->actions, stack->capacity * sizeof(Action));
    }
    stack->actions[stack->size++] = act;
}

Action pop_undo(UndoStack *stack) {
    if (stack->size == 0) return (Action){0};
    return stack->actions[--stack->size];
}

int compare_dates(Liver a, Liver b) {
    if (a.b_year != b.b_year) return a.b_year - b.b_year;
    if (a.b_mon != b.b_mon) return a.b_mon - b.b_mon;
    return a.b_day - b.b_day;
}

void insert_sorted(LinkedList *list, Liver new_liver) {
    Node *current = list->head;
    size_t index = 0;
    while (current) {
        if (compare_dates(new_liver, current->data) < 0) {
            break;
        }
        current = current->next;
        index++;
    }
    insert_at_list(list, index, new_liver);
}

void load_from_file(LinkedList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("File open error");
        return;
    }
    Liver temp;
    while (fscanf(f, "%s %s %s %d %d %d %c %lf", 
           temp.surname, temp.name, temp.patronymic, 
           &temp.b_day, &temp.b_mon, &temp.b_year, 
           &temp.gender, &temp.income) == 8) {
        
        if (validate_liver(&temp)) {
            insert_sorted(list, temp);
        }
    }
    fclose(f);
    printf("Data loaded.\n");
}

int main() {
    LinkedList list = create_list();
    UndoStack undo_stack = {malloc(10 * sizeof(Action)), 0, 10};
    
    FILE *f = fopen("data.txt", "w");
    if (f) {
        fprintf(f, "Ivanov Ivan Ivanovich 15 05 1990 M 50000.00\n");
        fprintf(f, "Petrova Anna Sergeevna 20 10 1995 W 60000.00\n");
        fprintf(f, "Sidorov Petr Petrovich 01 01 1980 M 75000.00\n");
        fclose(f);
    }

    printf("--- Loading from data.txt ---\n");
    load_from_file(&list, "data.txt");

    int choice;
    while(1) {
        printf("\n1. Print List\n2. Add Liver (Sorted)\n3. Delete at Index\n4. Undo (N/2)\n0. Exit\n> ");
        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) break;
        
        if (choice == 1) {
            Node *curr = list.head;
            int i = 0;
            while(curr) {
                printf("[%d] ", i++);
                print_liver(&curr->data);
                curr = curr->next;
            }
        }
        else if (choice == 2) {
            Liver l;
            printf("Enter: Surname Name Patronymic D M Y Gender(M/W) Income:\n");
            scanf("%s %s %s %d %d %d %c %lf", 
                  l.surname, l.name, l.patronymic, 
                  &l.b_day, &l.b_mon, &l.b_year, &l.gender, &l.income);
            
            if (validate_liver(&l)) {
                insert_sorted(&list, l);
                Action act; 
                act.type = CMD_ADD; 
                act.data = l; 
                push_undo(&undo_stack, act); 
            } else {
                printf("Invalid data (Check latin letters/dates)!\n");
            }
        }
        else if (choice == 3) {
            size_t idx;
            printf("Index to delete: ");
            scanf("%zu", &idx);
            if (idx < list.size) {
                Liver deleted = get_at_list(&list, idx);
                delete_at_list(&list, idx);
                Action act;
                act.type = CMD_DELETE;
                act.data = deleted;
                push_undo(&undo_stack, act);
            } else {
                printf("Invalid index!\n");
            }
        }
        else if (choice == 4) {
            size_t n = undo_stack.size;
            size_t to_undo = n / 2;
            printf("Undoing %zu operations...\n", to_undo);
            
            for (size_t i = 0; i < to_undo; i++) {
                Action act = pop_undo(&undo_stack);
                if (act.type == CMD_ADD) {
                    Node *curr = list.head;
                    size_t pos = 0;
                    while(curr) {
                        if (strcmp(curr->data.surname, act.data.surname) == 0 &&
                            curr->data.b_year == act.data.b_year) {
                            delete_at_list(&list, pos);
                            break;
                        }
                        curr = curr->next;
                        pos++;
                    }
                } 
                else if (act.type == CMD_DELETE) {
                    insert_sorted(&list, act.data);
                }
            }
        }
    }
    
    delete_list(&list);
    free(undo_stack.actions);
    return 0;
}