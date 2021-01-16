//
// Created by ultimatum on 16.01.2021.
//

#ifndef LOCK_FREE_QUEUE_BASKETSLFQ_H
#define LOCK_FREE_QUEUE_BASKETSLFQ_H

struct node_t;

struct baskets_pointer_t {
    struct baskets_node_t *ptr;
    unsigned short deleted;
    unsigned int tag;
};

struct baskets_node_t {
    void *value;
    struct baskets_pointer_t* next;
};

struct baskets_queue {
    struct baskets_pointer_t* tail;
    struct baskets_pointer_t* head;
};

void initBasketsQueue(struct baskets_queue *queue);

int baskets_enqueue(struct baskets_queue* basketsQueue, void* data);

void* baskets_dequeue(struct baskets_queue* basketsQueue);

#endif //LOCK_FREE_QUEUE_BASKETSLFQ_H
