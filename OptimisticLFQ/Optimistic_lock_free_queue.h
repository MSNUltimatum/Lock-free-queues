//
// Created by Ultimatum on 28.10.2020.
//
#ifndef LOCK_FREE_QUEUE_OPTIMISTIC_LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_OPTIMISTIC_LOCK_FREE_QUEUE_H

struct node;
struct pointer{
    struct node* ptr;
    unsigned int tag;
};

struct node{
    void* value;
    struct pointer next;
    struct pointer prev;
};

struct queue{
    struct pointer tail;
    struct pointer head;
};

void enqueueOpt(struct queue* q, void* val);
void* dequeueOpt(struct queue* q);
void initQueue(struct queue *queue);
#endif //LOCK_FREE_QUEUE_OPTIMISTIC_LOCK_FREE_QUEUE_H
