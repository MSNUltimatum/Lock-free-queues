//
// Created by ultimatum on 16.01.2021.
//

#include "BasketsLFQ.h"
#include "../HelpStruct/exp_backoff.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define CAS __sync_bool_compare_and_swap
#define MAX_HOPS 3

void init_queue(struct queue_t *q) {
    struct node_t *nd = calloc(sizeof(struct node_t), 1);
    nd->next.ptr = NULL;
    nd->next.deleted = 0;
    nd->next.tag = 0;
    q->tail.ptr = nd;
    q->tail.deleted = 0;
    q->tail.tag = 0;
    q->head.ptr = nd;
    q->head.deleted = 0;
    q->head.tag = 0;
}

void backoff_scheme() {
     nanosleep((const struct timespec[]){{0, 500L}}, NULL);
//    usleep(1);
}

bool baskets_enqueue(struct queue_t *q, int val) {
    struct node_t *nd = calloc(sizeof(struct node_t), 1);
    nd->value = val;
    while (true) {
        struct pointer_t tail = q->tail;
        struct pointer_t next = tail.ptr->next;
        if (tail.ptr == q->tail.ptr) {
            if (next.ptr == NULL) {
                nd->next.ptr = NULL;
                nd->next.deleted = 0;
                nd->next.tag = tail.tag + 2;
                if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
                    CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
                    CAS(&tail.ptr->next.tag, next.tag, tail.tag + 1)) {
                    CAS(&q->tail.ptr, tail.ptr, nd);
                    CAS(&q->tail.deleted, tail.deleted, 0);
                    CAS(&q->tail.tag, tail.tag, tail.tag + 1);
                    return true;
                }
                next = tail.ptr->next;
                while ((next.tag == tail.tag + 1) && (!next.deleted)) {
                    backoff_scheme();
                    nd->next = next;
                    if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
                        CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
                        CAS(&tail.ptr->next.tag, next.tag, tail.tag + 1)) {
                        return true;
                    }
                    next = tail.ptr->next;
                }
            } else {
                while ((next.ptr->next.ptr != NULL) && (q->tail.ptr == tail.ptr)) {
                    next = next.ptr->next;
                }
                CAS(&q->tail.ptr, tail.ptr, nd);
                CAS(&q->tail.deleted, tail.deleted, 0);
                CAS(&q->tail.tag, tail.tag, tail.tag + 1);
            }
        }
    }
    return false;
}

void free_chain(struct queue_t *q, struct pointer_t head, struct pointer_t new_head) {
    if (CAS(&q->head.ptr, head.ptr, new_head.ptr) &&
        CAS(&q->head.deleted, head.deleted, 0) &&
        CAS(&q->head.tag, head.tag, head.tag + 1)) {
        while (head.ptr != new_head.ptr) {
            struct pointer_t next = head.ptr->next;
            // reclaim_node(head.ptr); // not sure what this is from paper
            head = next;
        }
    }
}

int baskets_dequeue(struct queue_t *q) {
    while (true) {
        struct pointer_t head = q->head;
        struct pointer_t tail = q->tail;
        struct pointer_t next = head.ptr->next;
        if (head.ptr == q->head.ptr) {
            if (head.ptr == tail.ptr) {
                if (next.ptr == NULL) return -1;
                while ((next.ptr->next.ptr != NULL) && (q->tail.ptr == tail.ptr))
                    next = next.ptr->next;
                CAS(&q->tail.ptr, tail.ptr, next.ptr);
                CAS(&q->tail.deleted, tail.deleted, 0);
                CAS(&q->tail.tag, tail.tag, tail.tag + 1);
            } else {
                struct pointer_t iter = head;
                size_t hops = 0;
                while ((next.deleted && iter.ptr != tail.ptr) && (q->head.ptr == head.ptr)) {
                    iter = next;
                    next = iter.ptr->next;
                    hops++;
                }
                if (q->head.ptr != head.ptr) continue;
                else if (iter.ptr == tail.ptr) free_chain(q, head, iter);
                else {
                    size_t value = next.ptr->value;
                    if (CAS(&iter.ptr->next.ptr, next.ptr, next.ptr) &&
                        CAS(&iter.ptr->next.deleted, next.deleted, 1) &&
                        CAS(&iter.ptr->next.tag, next.tag, next.tag + 1)) {
                        if (hops >= MAX_HOPS) free_chain(q, head, iter);
                        return value;
                    }
                    backoff_scheme();
                }
            }
        }
    }
    return -1;
}