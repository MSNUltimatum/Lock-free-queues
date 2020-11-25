//
// Created by Ultimatum on 25.10.2020.
//
#include "MS_queue.h"
#include <malloc.h>
#include <zconf.h>
#include <stdatomic.h>

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
        if (lfqueue1->tail != tail) continue;
        struct queue_node *next = tail->next;
        if(atomic_load(&lfqueue1->size) < atomic_load(&lfqueue1->maxQueueSize)) {
            if (tail == lfqueue1->tail) {
                if (next == NULL) {
                    if (CAS(&tail->next, next, newNode)) {
                        INCREMENT(&lfqueue1->size, 1, __ATOMIC_SEQ_CST);
                        break;
                    }
                } else {
                    CAS(&lfqueue1->tail, tail, next);
                }
            }
        } else {
            continue;
        }
    }
    CAS(&lfqueue1->tail, tail, newNode);
    hprec->HP[0] = NULL;
}

void *dequeue(lfqueue *lfqueue1, struct hprec_t* hprec, HP* hp) {
    void *result;
    node_q *head;
    while (1) {
        head = lfqueue1->head;
        hprec->HP[0] = head;
        if(lfqueue1->head != head) continue;
        node_q *tail = lfqueue1->tail;
        node_q *nextHead = head->next;
        hprec->HP[1] = nextHead;
        if(nextHead == NULL){//TODO backoff
            continue;
        }
        if (head == lfqueue1->head) {
            if (head == tail) {
                if (nextHead == NULL) {
                    return QUEUE_EMPTY;
                }
                CAS(&lfqueue1->tail, tail, nextHead);
            } else {
                result = nextHead->data;
                if (CAS(&lfqueue1->head, head, nextHead)) {
                    INCREMENT(&lfqueue1->size, -1, __ATOMIC_SEQ_CST);
                    break;
                }
                retire_node(hp, hprec, head);
                hprec->HP[0] = NULL;
                hprec->HP[1] = NULL;
            }
        }
    }
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