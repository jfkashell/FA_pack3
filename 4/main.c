#include "lab4.h"
#include <stdlib.h>
#include <stdio.h>

PostOffice *offices = NULL;
size_t offices_count = 0;
 
unsigned long generate_letter_id() {
    static unsigned long id_counter = 1;
    return id_counter++;
}

PostOffice* find_office(int id) {
    for (size_t i = 0; i < offices_count; i++) {
        if (offices[i].id == id) return &offices[i];
    }
    return NULL;
}

void add_connection(int id1, int id2) {
    PostOffice *p1 = find_office(id1);
    PostOffice *p2 = find_office(id2);
    if (!p1 || !p2) return;

    p1->connections_count++;
    p1->connected_ids = realloc(p1->connected_ids, p1->connections_count * sizeof(int));
    p1->connected_ids[p1->connections_count - 1] = id2;

    p2->connections_count++;
    p2->connected_ids = realloc(p2->connected_ids, p2->connections_count * sizeof(int));
    p2->connected_ids[p2->connections_count - 1] = id1;
}

void create_office(int id, size_t cap) {
    offices_count++;
    offices = realloc(offices, offices_count * sizeof(PostOffice));
    PostOffice *po = &offices[offices_count - 1];
    po->id = id;
    po->max_capacity = cap;
    po->mail_storage = create_heap(10);
    po->connected_ids = NULL;
    po->connections_count = 0;
    printf("Office %d created.\n", id);
}

void create_letter(int type_idx, int priority, int sender, int receiver) {
    PostOffice *sender_po = find_office(sender);
    if (!sender_po) {
        printf("Error: Sender office %d not found.\n", sender);
        return;
    }

    if (size_heap(&sender_po->mail_storage) >= sender_po->max_capacity) {
        printf("Error: Sender office full. Letter rejected.\n");
        return;
    }

    Letter l;
    l.id = generate_letter_id();
    l.priority = priority;
    l.sender_id = sender;
    l.receiver_id = receiver;
    l.status = STATUS_PROCESSING;
    snprintf(l.type, 32, "Type-%d", type_idx);
    snprintf(l.tech_data, 100, "Content of letter %lu", l.id);
    l.created_at = time(NULL);

    push_heap(&sender_po->mail_storage, l);
    printf("Letter %lu created at Office %d -> To %d (Priority: %d)\n", 
           l.id, sender, receiver, priority);
}

void log_event(const char *msg) {
    FILE *f = fopen("mail_log.txt", "a");
    if (f) {
        fprintf(f, "[LOG] %s\n", msg);
        fclose(f);
    }
}

void simulation_step() {
    char log_buf[256];
    printf("\n--- Simulation Step ---\n");

    for (size_t i = 0; i < offices_count; i++) {
        PostOffice *po = &offices[i];
        if (is_empty_heap(&po->mail_storage)) continue;

        Letter mail = pop_heap(&po->mail_storage);

        if (po->id == mail.receiver_id) {
            printf("[DELIVERED] Letter %lu reached destination %d!\n", mail.id, po->id);
            snprintf(log_buf, 256, "Letter %lu DELIVERED at %d", mail.id, po->id);
            log_event(log_buf);
            continue;
        }

        if (po->connections_count == 0) {
            printf("[STUCK] Letter %lu at %d has nowhere to go.\n", mail.id, po->id);
            push_heap(&po->mail_storage, mail);
            continue;
        }

        int target_id = -1;
        PostOffice *target_po = NULL;

        for (size_t j = 0; j < po->connections_count; j++) {
            if (po->connected_ids[j] == mail.receiver_id) {
                target_id = po->connected_ids[j];
                break;
            }
        }

        if (target_id == -1) {
            target_id = po->connected_ids[0];
        }

        target_po = find_office(target_id);

        if (target_po && size_heap(&target_po->mail_storage) < target_po->max_capacity) {
            push_heap(&target_po->mail_storage, mail);
            printf("[MOVED] Letter %lu: Office %d -> Office %d\n", mail.id, po->id, target_id);
            snprintf(log_buf, 256, "Letter %lu MOVED %d->%d", mail.id, po->id, target_id);
            log_event(log_buf);
        } else {
            push_heap(&po->mail_storage, mail);
            printf("[WAIT] Letter %lu stuck at %d (Target %d full/closed)\n", mail.id, po->id, target_id);
        }
    }
}

void print_all_letters() {
    printf("\n--- Current Letters Status ---\n");
    for (size_t i = 0; i < offices_count; i++) {
        PostOffice *po = &offices[i];
        if (!is_empty_heap(&po->mail_storage)) {
            Letter top = peek_heap(&po->mail_storage);
            printf("Office %d: %zu letters. Top Priority: %d (ID: %lu)\n", 
                   po->id, po->mail_storage.size, top.priority, top.id);
        } else {
            printf("Office %d: Empty\n", po->id);
        }
    }
}

void run_heap_tests() {
    printf("[TEST] Heap Logic... ");
    Heap h = create_heap(5);
    
    Letter l1 = {1, "", 10, 0, 0, "", 0, 0};
    Letter l2 = {2, "", 50, 0, 0, "", 0, 0};
    Letter l3 = {3, "", 5, 0, 0, "", 0, 0};

    push_heap(&h, l1);
    push_heap(&h, l2);
    push_heap(&h, l3);

    Letter top = peek_heap(&h);
    if (top.priority != 50) {
        printf("FAIL (Top priority is %d, expected 50)\n", top.priority);
        return;
    }

    Letter p1 = pop_heap(&h);
    Letter p2 = pop_heap(&h); 
    Letter p3 = pop_heap(&h); 

    if (p1.priority == 50 && p2.priority == 10 && p3.priority == 5) {
        printf("PASS\n");
    } else {
        printf("FAIL (Order: %d %d %d)\n", p1.priority, p2.priority, p3.priority);
    }
    delete_heap(&h);
}

int main(int argc, char *argv[]) {
    run_heap_tests();

    if (argc >= 2) {
        FILE *f = fopen(argv[1], "r");
        if (f) {
            int id1, id2;
            create_office(1, 5);
            create_office(2, 5);
            create_office(3, 5);
            
            while (fscanf(f, "%d %d", &id1, &id2) == 2) {
                add_connection(id1, id2);
                printf("Connected %d <-> %d\n", id1, id2);
            }
            fclose(f);
        } else {
            printf("Could not open mapping file.\n");
        }
    } else {
        printf("No file provided. Using default config.\n");
        create_office(1, 3);
        create_office(2, 3);
        create_office(3, 3);
        add_connection(1, 2);
        add_connection(2, 3);
    }

    int choice;
    while(1) {
        printf("\n1. Add Letter\n2. Step Simulation (0.2s)\n3. List Status\n0. Exit\n> ");
        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) break;

        switch(choice) {
            case 1: {
                int from, to, prio;
                printf("From ID, To ID, Priority: ");
                scanf("%d %d %d", &from, &to, &prio);
                create_letter(1, prio, from, to);
                break;
            }
            case 2:
                simulation_step();
                break;
            case 3:
                print_all_letters();
                break;
        }
    }

    for (size_t i = 0; i < offices_count; i++) {
        delete_heap(&offices[i].mail_storage);
        free(offices[i].connected_ids);
    }
    free(offices);

    return 0;
}