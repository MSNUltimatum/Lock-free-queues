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

#define CAS __sync_bool_compare_and_swap
#define MAX_HOPS 3

struct baskets_node_t *new_baskets_node();

void allocate_ptr(struct baskets_pointer_t *pointer, struct baskets_node_t *pNode, int deleted, unsigned int tag);

struct baskets_pointer_t *new_baskets_pointer(struct baskets_node_t *pNode, int deleted, unsigned int tag);

void free_chain(struct baskets_queue *pQueue, struct baskets_pointer_t *head, struct baskets_pointer_t *new_head);

bool eq(struct baskets_pointer_t *pPointer, struct baskets_pointer_t *pPointer1);

void initBasketsQueue(struct baskets_queue *queue) {
    struct baskets_node_t *nd = new_baskets_node();
    queue->tail = calloc(sizeof(struct baskets_pointer_t), 1);
    queue->head = calloc(sizeof(struct baskets_pointer_t), 1);
    allocate_ptr(nd->next, NULL, 0, 0);
    allocate_ptr(queue->tail, nd, 0, 0);
    allocate_ptr(queue->head, nd, 0, 0);
}

int baskets_enqueue(struct baskets_queue *basketsQueue, void *data) {
    struct baskets_node_t *nd = new_baskets_node();
    nd->value = data;
    while (1) {
        struct baskets_pointer_t *tail = basketsQueue->tail;
        struct baskets_pointer_t *next = tail->ptr->next;
        if (tail == basketsQueue->tail) {
            if (next->ptr == NULL) {
                allocate_ptr(nd->next, NULL, 0, tail->tag + 2);
                if (CAS(&(tail->ptr->next), next, new_baskets_pointer(nd, 0, tail->tag + 1))) {
                    CAS(&(basketsQueue->tail), tail, new_baskets_pointer(nd, 0, tail->tag + 1));
                    return 1;
                }
                next = tail->ptr->next;
                while ((next->tag == tail->tag + 1) && (!next->deleted)) {
//                    backoff(5, 1000, 2, 12500); TODO make normal
                    nd->next = next;
                    if (CAS(&tail->ptr->next, next, new_baskets_pointer(nd, 0, tail->tag + 1)))
                        return 1;
                    next = tail->ptr->next;
                }
            } else {
                while (tail == basketsQueue->tail && next->ptr->next->ptr != NULL) {
                    next = next->ptr->next;
                }
                CAS(&(basketsQueue->tail), tail, new_baskets_pointer(next->ptr, 0, tail->tag + 1));
            }
        }
    }
}

bool eq(struct baskets_pointer_t *pPointer, struct baskets_pointer_t *pPointer1) {
    return pPointer->ptr == pPointer1->ptr && pPointer->tag == pPointer1->tag &&
           pPointer->deleted == pPointer1->deleted;
}

void *baskets_dequeue(struct baskets_queue *basketsQueue) {
    while (1) {
        struct baskets_pointer_t *head = basketsQueue->head;
        struct baskets_pointer_t *tail = basketsQueue->tail;
        struct baskets_pointer_t *next = head->ptr->next;
        if (head == basketsQueue->head) {
            if (head->ptr == tail->ptr) {
                if (next->ptr == NULL) {
                    return -1;
                }
                while ((next->ptr->next->ptr != NULL) &&
                       (tail == basketsQueue->tail)) {
                    next = next->ptr->next;
                }
                CAS(&(basketsQueue->tail), tail, new_baskets_pointer(next->ptr, 0, tail->tag + 1));
            } else {
                struct baskets_pointer_t *iter = head;
                int hops = 0;
                while ((next->deleted && iter->ptr != tail->ptr) &&
                       (head == basketsQueue->head)) {
                    iter = next;
                    next = iter->ptr->next;
                    hops++;
                }

                if (head != basketsQueue->head) {
                    continue;
                } else if (iter->ptr == tail->ptr) {
                    free_chain(basketsQueue, head, iter);
                } else {
                    void *val = next->ptr->value;
                    if (CAS(&(iter->ptr->next), next, new_baskets_pointer(next->ptr, 1, next->tag + 1))) {
                        if (hops >= MAX_HOPS) {
                            free_chain(basketsQueue, head, next);
                        }
                        return val;
                    }
//                    backoff(5, 1000, 2, 12500);
                }
            }
        }
    }
}

void free_chain(struct baskets_queue *pQueue, struct baskets_pointer_t *head, struct baskets_pointer_t *new_head) {
    if (CAS(&(pQueue->head), head, new_baskets_pointer(new_head->ptr, 0, head->tag + 1))) {
        while (head->ptr != new_head->ptr) {
            struct baskets_pointer_t *next = head->ptr->next;
//            free(head->ptr);
//            head->ptr = NULL;
            head = next;
        }
    }
}

struct baskets_pointer_t *new_baskets_pointer(struct baskets_node_t *pNode, int deleted, unsigned int tag) {
    struct baskets_pointer_t *new_pointer_v = calloc(sizeof(struct baskets_pointer_t), 1);
    allocate_ptr(new_pointer_v, pNode, deleted, tag);
    return new_pointer_v;
}

void allocate_ptr(struct baskets_pointer_t *pointer, struct baskets_node_t *pNode, int deleted, unsigned int tag) {
    pointer->ptr = pNode;
    pointer->deleted = deleted;
    pointer->tag = tag;
}

struct baskets_node_t *new_baskets_node() {
    struct baskets_node_t *node = calloc(sizeof(struct baskets_node_t), 1);
    node->next = calloc(sizeof(struct baskets_pointer_t), 1);
    return node;
}