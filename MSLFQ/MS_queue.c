//
// Created by Ultimatum on 25.10.2020.
//
#include "MS_queue.h"
#include <malloc.h>
#include <stdatomic.h>
#include "../HelpStruct/exp_backoff.h"

#define CAS __sync_bool_compare_and_swap
#define INCREMENT __atomic_fetch_add
#define QUEUE_EMPTY 0

void initMSqueue(lfqueue *lfqueue1, int maxQueueSize) {
    node_q *dummy = malloc(sizeof(struct queue_node));
    dummy->next = NULL;
    lfqueue1->head = dummy;
    lfqueue1->tail = dummy;
    lfqueue1->size = 0;
    lfqueue1->maxQueueSize = maxQueueSize;
}

int enqueue(struct LFqueue *lfqueue1, void* data, struct hprec_t* hprec){
    struct queue_node *tail,  *newNode = calloc(1, sizeof(node_q));
    newNode->data = data;
    newNode->next = NULL;
    while (1) {
        tail = lfqueue1->tail;
        hprec->HP[0] = tail;
//        if(atomic_load(&lfqueue1->size) < atomic_load(&lfqueue1->maxQueueSize)) {
            if (lfqueue1->tail != tail) continue;
            struct queue_node *next = tail->next;
            if (lfqueue1->tail != tail) continue;
            if (next != NULL) {
                __sync_bool_compare_and_swap(&lfqueue1->tail, tail, next);
                continue;
            }
            if (__sync_bool_compare_and_swap(&tail->next, NULL, newNode)) {
                INCREMENT(&lfqueue1->size, 1, __ATOMIC_SEQ_CST);
                break;
            }
//        }
        backoff(5, 1000, 2, 1250);
    }
    CAS(&lfqueue1->tail, tail, newNode);
    hprec->HP[0] = NULL;
    return 0;
}

void *dequeue(lfqueue *lfqueue1, struct hprec_t* hprec, HP* hp) {
    void *result;
    node_q *head;
    while (1) {
        head = lfqueue1->head;
        hprec->HP[0] = head;
        if(lfqueue1->head != head) continue;
        struct queue_node *t = lfqueue1->tail;
        struct queue_node *next = head->next;
        hprec->HP[1] = next;
        if(lfqueue1->head != head) continue;
        if(next == NULL) return -1;
        if(head == t) {
            CAS(&lfqueue1->tail, t, next);
            continue;
        }
        result = next->data;
        if(CAS(&lfqueue1->head, head, next)) {
            INCREMENT(&lfqueue1->size, -1, __ATOMIC_SEQ_CST);
            break;
        }
        backoff(5, 1000, 2, 1250);
    }
    retire_node(hp, hprec, head);
    hprec->HP[0] = NULL;
    hprec->HP[1] = NULL;
    return result;
}

void freeQueue(lfqueue *lfqueue1) {
    node_q *head = lfqueue1->head;
    while (head->next != NULL) {
        node_q *tmp = head;
        head = head->next;
        free(tmp);
    }
    free(lfqueue1);
}