//
// Created by ultimatum on 16.01.2021.
//


#include "BasketsLFQ.h"


#include "BasketsLFQ.h"


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define CAS __sync_bool_compare_and_swap
#define MAX_HOPS 3

struct pointer_t *make_link_pointer(struct node_t *node, bool deleted, unsigned int tag) {
    struct pointer_t *pointer = calloc(sizeof(struct pointer_t), 1);
    unsigned long long pointer_struct = ((unsigned long long) tag << 1) | (unsigned long long) deleted;
    atomic_store(&pointer->cnt_val, (pointer_struct << 48) | (unsigned long long) node);
    return pointer;
}

struct pointer_t make_pointer(struct node_t *node, bool deleted, unsigned int tag) {
    struct pointer_t pointer;
    unsigned long long pointer_struct = ((unsigned long long) tag << 1) | (unsigned long long) deleted;
    atomic_store(&pointer.cnt_val, (pointer_struct << 48) | (unsigned long long) node);
    return pointer;
}

struct node_t *get_ptr(struct pointer_t pointer) {
    unsigned long long shifted = atomic_load(&pointer.cnt_val) << 16;
    shifted = shifted >> 16;
    struct node_t *ptr = (struct node_t *) (long) shifted;
    return ptr;
}

int get_tag(struct pointer_t pointer) {
    unsigned long long shifted = (unsigned long long) atomic_load(&pointer.cnt_val) >> 49;
    return (int) shifted;
}

int get_deleted(struct pointer_t pointer) {
    unsigned long long v = (unsigned long long) atomic_load(&pointer.cnt_val) >> 48;
    unsigned long long temp = 1;
    unsigned long long d = (v & temp);
    return (int) d;
}

bool my_CAS(struct pointer_t *pointer, unsigned long long cp1, unsigned long long cp2) {
    if (CAS(&pointer->cnt_val, cp1, cp2)) {
        return true;
    }
    return false;
}

struct node_t *new_baskets_node(int val) {
    struct node_t *nd = calloc(sizeof(struct node_t), 1);
    nd->value = val;
    return nd;
}

void backoff_scheme() {
    for (int i = 0; i < 10000; i++);
}

void baskets_init_queue(struct queue_t *queue) {
    struct node_t *nd = new_baskets_node(0);
    nd->next = make_link_pointer(NULL, 0, 0);
    struct pointer_t empty = make_pointer(nd, 0, 0);
    atomic_store(&queue->head.cnt_val, empty.cnt_val);
    atomic_store(&queue->tail.cnt_val, empty.cnt_val);
}

void free_chain(struct queue_t *queue, struct pointer_t *head, struct pointer_t *new_head) {
    struct pointer_t temp_node1 = make_pointer(get_ptr(*new_head), 0, get_tag(*head) + 1);
    struct pointer_t nxt = make_pointer(NULL, 0, 0);
    if (my_CAS(&queue->head, atomic_load(&(head->cnt_val)), atomic_load(&(temp_node1.cnt_val)))) {
        while (get_ptr(*head) != get_ptr(*new_head)) {
            atomic_store(&nxt.cnt_val, get_ptr(*head)->next->cnt_val);
            atomic_store(&head->cnt_val, nxt.cnt_val);
        }
    }
}

bool baskets_enqueue(struct queue_t *queue, int val) {
    struct node_t *nd = new_baskets_node(val);
    struct pointer_t nxt = make_pointer(NULL, 0, 0);
    struct pointer_t t = make_pointer(NULL, 0, 0);

    nd->next = make_link_pointer(NULL, 0, 0);
    while (1) {
        atomic_store(&t.cnt_val, queue->tail.cnt_val);
        atomic_store(&nxt.cnt_val, get_ptr(t)->next->cnt_val);
        if (atomic_load(&t.cnt_val) == atomic_load(&queue->tail.cnt_val)) {
            if (get_ptr(nxt) == NULL) {
                struct pointer_t temp_node1 = make_pointer(NULL, 0, get_tag(t) + 2);
                atomic_store(&nd->next->cnt_val, temp_node1.cnt_val);
                struct pointer_t temp_node = make_pointer(nd, 0, get_tag(t) + 1);
                if (my_CAS(get_ptr(t)->next, atomic_load(&nxt.cnt_val), atomic_load(&temp_node.cnt_val))) {
                    my_CAS(&queue->tail, atomic_load(&t.cnt_val), atomic_load(&temp_node.cnt_val));
                    return true;
                }
                atomic_store(&nxt.cnt_val, get_ptr(t)->next->cnt_val);
                while (get_tag(nxt) == get_tag(t) + 1 && !(get_deleted(nxt))) {
                    backoff_scheme();
                    atomic_store(&nd->next->cnt_val, nxt.cnt_val);
                    struct pointer_t temp_node3 = make_pointer(nd, 0, get_tag(t) + 1);
                    if (my_CAS(get_ptr(t)->next, nxt.cnt_val, temp_node3.cnt_val)) {
                        return true;
                    }
                    atomic_store(&nxt.cnt_val, get_ptr(t)->next->cnt_val);
                }
            } else {
                while ((get_ptr(*get_ptr(nxt)->next) != NULL) &&
                       (atomic_load(&queue->tail.cnt_val) == atomic_load(&t.cnt_val))) {
                    atomic_store(&nxt.cnt_val, get_ptr(nxt)->next->cnt_val);
                }
                struct pointer_t temp_node4 = make_pointer(get_ptr(nxt), 0, get_tag(t) + 1);
                my_CAS(&queue->tail, atomic_load(&t.cnt_val), atomic_load(&temp_node4.cnt_val));
            }
        }
    }
}


int baskets_dequeue(struct queue_t *queue) {
    struct pointer_t head = make_pointer(NULL, 0, 0);
    struct pointer_t tail = make_pointer(NULL, 0, 0);
    struct pointer_t next = make_pointer(NULL, 0, 0);
    struct pointer_t iteration = make_pointer(NULL, 0, 0);
    int value;
    int hops;
    while (1) {
        atomic_store(&tail.cnt_val, queue->tail.cnt_val);
        atomic_store(&head.cnt_val, queue->head.cnt_val);
        atomic_store(&next.cnt_val, get_ptr(head)->next->cnt_val);

        if (atomic_load(&head.cnt_val) == atomic_load(&queue->head.cnt_val)) {
            if (get_ptr(head) == get_ptr(tail)) {
                if (get_ptr(next) == NULL)
                    return -1;
                while (get_ptr(*get_ptr(next)->next) != NULL &&
                       atomic_load(&queue->tail.cnt_val) == atomic_load(&tail.cnt_val)) {
                    atomic_store(&next.cnt_val, get_ptr(next)->next->cnt_val);
                }
                struct pointer_t temp_node = make_pointer(get_ptr(next), 0, get_tag(tail) + 1);
                my_CAS(&queue->tail, atomic_load(&tail.cnt_val), atomic_load(&temp_node.cnt_val));
            } else {
                atomic_store(&iteration.cnt_val, head.cnt_val);
                hops = 0;

                while ((get_deleted(next)) && (get_ptr(iteration) != get_ptr(tail)) &&
                       (atomic_load(&queue->head.cnt_val) == atomic_load(&head.cnt_val))) {
                    atomic_store(&iteration.cnt_val, next.cnt_val);
                    atomic_store(&next.cnt_val, get_ptr(iteration)->next->cnt_val);
                    hops++;

                }
                if (atomic_load(&queue->head.cnt_val) != atomic_load(&head.cnt_val)) {
                    continue;
                } else if (get_ptr(iteration) == get_ptr(tail)) {
                    free_chain(queue, &head, &iteration);
                } else {
                    value = get_ptr(next)->value;
                    struct pointer_t temp_node2 = make_pointer(get_ptr(next), 1, get_tag(next) + 1);
                    if (my_CAS(get_ptr(iteration)->next, atomic_load(&next.cnt_val),
                               atomic_load(&temp_node2.cnt_val))) {
                        if (hops >= MAX_HOPS) {
                            free_chain(queue, &head, &next);
                        }
                        return value;
                    }
                    backoff_scheme();
                }
            }
        }
    }
}