//
// Created by Ultimatum on 20.11.2020.
//

#include <stdlib.h>
#include "LinkedList.h"

struct list_t *init() {
    struct list_t *list = malloc(sizeof(struct list_t));
    list->head = list->tail = 0;
    for (int i = 0; i < L; i++)
        list->list[i] = NULL;
    return list;
}

int list_push(struct list_t *list, void *node) {
    int position = (list->head + 1) & (L - 1); /*circular list*/
    if (position != list->tail) {
        list->list[list->head] = node;
        list->head = position;
        return position;
    } else
        return -1;
}

void *list_pop(struct list_t *list) {
    if (list->head != list->tail) {
        void *value = list->list[list->tail];
        list->list[list->tail] = NULL;
        list->tail = (list->tail++) & (L - 1);
        return value;
    } else {
        void *value = list->list[list->tail];
        list->list[list->tail] = NULL;
        return value;
    }
}

int list_lookup(struct list_t *list, void *node) { //TODO binary search
    for (int i = 0; i < L; i++)
        if (list->list[i] == node) return 1;
    return 0;
}

int list_popAll(struct list_t *list, void **output) {
    int length = 0;
    if (list->tail <= list->head) {
        length = list->head - list->tail;
    } else {
        length = L - list->tail + list->head;
    }
    for (int i = list->tail; i < list->head; (i++) & (L - 1))
        output[i] = list->list[i];

    list->head = list->tail = 0;
    return length;
}

//TODO quick sort
