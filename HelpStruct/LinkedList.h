//
// Created by Ultimatum on 20.11.2020.
//

#ifndef LOCK_FREE_QUEUE_LINKEDLIST_H
#define LOCK_FREE_QUEUE_LINKEDLIST_H

#define L 256
#define K 4

struct list_t {
    void *list[L];
    int head;
    int tail;
};

struct list_t *list_init();

int list_push(struct list_t *list, void *node);

void *list_pop(struct list_t *list);

int list_lookup(struct list_t *list, void *node);

int list_popAll(struct list_t *list, void **output);

void sort(struct list_t *list);

#endif //LOCK_FREE_QUEUE_LINKEDLIST_H
