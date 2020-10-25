//
// Created by Ultimatum on 25.10.2020.
//

#ifndef LOCK_FREE_QUEUE_QUEUE_H
#define LOCK_FREE_QUEUE_QUEUE_H

typedef struct queue_node{
    void* data;
    struct queue_node *next;
} node_q;

typedef struct LFqueue{
    struct queue_node *head;
    struct queue_node *tail;
    unsigned int size;
} lfqueue;

void init(lfqueue *lfqueue);
int enqueue(lfqueue *lfqueue, void* data);
void *dequeue(lfqueue *lfqueue);
#endif //LOCK_FREE_QUEUE_QUEUE_H
