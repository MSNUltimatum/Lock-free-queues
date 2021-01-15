//
// Created by Ultimatum on 25.10.2020.
//

#ifndef LOCK_FREE_QUEUE_MS_QUEUE_H
#define LOCK_FREE_QUEUE_MS_QUEUE_H

#include "../HelpStruct/HP.h"

typedef struct queue_node {
    void *data;
    struct queue_node *next;
} node_q;

typedef struct LFqueue {
    volatile struct queue_node *head;
    volatile struct queue_node *tail;
    unsigned int maxQueueSize;
    unsigned int size;
} lfqueue;

void initMSqueue(lfqueue *lfqueue1, int maxQueueSize);

int enqueue(lfqueue *lfqueue1, void *data, struct hprectype *hprec);

void *dequeue(lfqueue *lfqueue1, struct hprectype *hprect, HP *hp);

void freeQueue(lfqueue *lfqueue1);

#endif //LOCK_FREE_QUEUE_MS_QUEUE_H
