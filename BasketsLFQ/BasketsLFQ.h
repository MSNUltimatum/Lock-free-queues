//
// Created by ultimatum on 16.01.2021.
//

#ifndef LOCK_FREE_QUEUE_BASKETSLFQ_H
#define LOCK_FREE_QUEUE_BASKETSLFQ_H

#include <stdbool.h>
#include <glob.h>
#include <stdatomic.h>

struct pointer_t {
    unsigned long long cnt_val;
};

struct node_t {
    int value;
    struct pointer_t *next;
};
struct queue_t {
    struct pointer_t tail;
    struct pointer_t head;
};


void baskets_init_queue(struct queue_t *q);

bool baskets_enqueue(struct queue_t *q, int val);

int baskets_dequeue(struct queue_t *q);

#endif //LOCK_FREE_QUEUE_BASKETSLFQ_H
