//
// Created by Ultimatum on 20.11.2020.
//

#ifndef LOCK_FREE_QUEUE_LIST_H
#define LOCK_FREE_QUEUE_LIST_H

#include <stdbool.h>

#define K 4
#define L 256

struct listtype {
    void *list[L];
    int head;
    int tail;
};

struct listtype *list_init();

int list_push(struct listtype *list, void *node);

void *list_pop(struct listtype *list);

bool list_lookup(struct listtype *list, void *node);

int list_popall(struct listtype *list, void **output);

#endif //LOCK_FREE_QUEUE_LIST_H
