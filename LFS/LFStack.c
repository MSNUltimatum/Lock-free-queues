//
// Created by egrsa on 19.11.2020.
//

#include <stdlib.h>
#include "LFStack.h"


int lfs_init(lfstack_t *lstack, int max_size) {
    struct lfs_head head_init = {0, NULL};
    lstack->head = ATOMIC_VAR_INIT(head_init);
    lstack->size = ATOMIC_VAR_INIT(0);

    lstack->node_buffer = malloc(max_size * sizeof(struct lfs_node));
    if (lstack->node_buffer == NULL)
        return -1;
    for (size_t i = 0; i < max_size - 1; i++)
        lstack->node_buffer[i].next = lstack->node_buffer + i + 1;
    lstack->node_buffer[max_size - 1].next = NULL;
    struct lfs_head free_init = {0, lstack->node_buffer};
    lstack->free = ATOMIC_VAR_INIT(free_init);
    return 0;
}

struct lfs_node *pop(struct lfs_head *head) {
    struct lfs_head orig = atomic_load(head);
    struct lfs_head next;
    do {
        if (orig.node == NULL)
            return NULL;
        next.aba = orig.aba + 1;
        next.node = orig.node->next;
    } while (!atomic_compare_exchange_weak(head, &orig, next));
    return orig.node;
}

static void push(struct lfs_head *head, struct lfs_node *node) {
    struct lfs_head orig = atomic_load(head);
    struct lfs_head next;
    do {
        node->next = orig.node;
        next.aba = orig.aba + 1;
        next.node = node;
    } while (!atomic_compare_exchange_weak(head, &orig, next));
}

int lfs_push(lfstack_t *lstack, void *data) {
    struct lfs_node *node;
    while (1) {
        if ((node = pop(&lstack->free)) != NULL) {
            node->data = data;
            push(&lstack->head, node);
            atomic_fetch_add(&lstack->size, 1);
            break;
        }
    }
    return 0;
}

void *lfs_pop(lfstack_t *lstack) {
    void *value = NULL;
    struct lfs_node *node;
    while (1) {
        if ((node = pop(&lstack->head)) != NULL) {
            atomic_fetch_sub(&lstack->size, 1);
            value = node->data;
            push(&lstack->free, node);
            break;
        }
    }
    return value;
}
