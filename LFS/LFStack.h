//
// Created by egrsa on 19.11.2020.
//

#ifndef LOCK_FREE_QUEUE_LFSTACK_H
#define LOCK_FREE_QUEUE_LFSTACK_H

#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>

struct lfs_node {
    void *data;
    struct lfs_node *next;
};

struct lfs_head {
    uintptr_t aba;
    struct lfs_node *node;
};

typedef struct {
     struct lfs_node *node_buffer;
    _Atomic struct lfs_head head;
    _Atomic struct lfs_head free;
    _Atomic int size;
} lfstack_t;

static inline size_t lstack_size(lfstack_t *lstack)
{
    return atomic_load(&lstack->size);
}

static void lst_free(lfstack_t *lstack) {
    free(lstack->node_buffer);
}

int lfs_init(lfstack_t *lstack, int max_size);

int lfs_push(lfstack_t *lstack, void *value);

void *lfs_pop(lfstack_t *lstack);

#endif //LOCK_FREE_QUEUE_LFSTACK_H
