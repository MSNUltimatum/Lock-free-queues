//
// Created by Ultimatum on 25.10.2020.
//
#include "MS_queue.h"
#include <malloc.h>
#include <stdatomic.h>
#include "../HelpStruct/exp_backoff.h"

#define CAS __sync_bool_compare_and_swap
#define QUEUE_EMPTY 0

void initMSqueue(lfqueue *lfqueue1, int maxQueueSize) {
    node_q *dummy = malloc(sizeof(node_q));
    dummy->next = NULL;
    lfqueue1->head = dummy;
    lfqueue1->tail = dummy;
    lfqueue1->size = 0;
    lfqueue1->maxQueueSize = maxQueueSize;
}

node_q *queue_newnode(int data) {
    node_q *node = malloc(sizeof(node_q));
    node->next = NULL;
    node->data = data;
    return node;
}

int enqueue(lfqueue *q, void *d, struct hprectype *hprect) {
    node_q *node = queue_newnode(d);
    node_q *t;
    while (true) {
        t = q->tail;
        hprect->HP[0] = t;
        if (q->tail != t) continue;
        node_q *next = t->next;
        if (q->tail != t) continue;
        if (next != NULL) {
            __sync_bool_compare_and_swap(&q->tail, t, next);
            continue;
        }
        if (__sync_bool_compare_and_swap(&t->next, NULL, node)) break;
    }
    __sync_bool_compare_and_swap(&q->tail, t, node);
    hprect->HP[0] = NULL;
}

void *dequeue(lfqueue *q, struct hprectype *hprect, HP *hp) {
    node_q *h;
    int data;
    while (true) {
        h = q->head;
        hprect->HP[0] = h;
        if (q->head != h) continue;
        node_q *t = q->tail;
        node_q *next = h->next;
        hprect->HP[1] = next;
        if (q->head != h) continue;
        if (next == NULL) return -1;
        if (h == t) {
            __sync_bool_compare_and_swap(&q->tail, t, next);
            continue;
        }
        data = next->data;
        if (__sync_bool_compare_and_swap(&q->head, h, next)) break;
    }
    retire_node(hp, hprect, (void *) h);

    hprect->HP[0] = NULL;
    hprect->HP[1] = NULL;
    return data;
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