//
// Created by Ultimatum on 20.11.2020.
//

#include <malloc.h>
#include "list.h"

struct listtype *list_init() {
    struct listtype *list = malloc(sizeof(struct listtype));
    list->head = list->tail = 0;
    for (int i = 0; i < L; i++)
        list->list[i] = NULL;
    return list;
}

int list_push(struct listtype *list, void *node) {
    int pos = (list->head + 1) & (L - 1); /*circular list*/
    if (pos != list->tail) {
        list->list[list->head] = node;
        list->head = pos;
        return pos;
    } else return -1;
}

void *list_pop(struct listtype *list) {
    if (list->head != list->tail) {
        void *tmpval = list->list[list->tail];
        list->list[list->tail] = NULL;
        list->tail = (list->tail++) & (L - 1);
        return tmpval;
    } else {
        void *tmpval = list->list[list->tail];
        list->list[list->tail] = NULL;
        return tmpval;
    }
}

bool list_lookup(struct listtype *list, void *node) {
    for (int i = 0; i < L; i++)
        if (list->list[i] == node) return true;
    return false;
}


int list_popall(struct listtype *list, void **output) {
    int length = 0;
    if (list->tail <= list->head) length = list->head - list->tail;
    else length = L - list->tail + list->head;
    for (int i = list->tail; i < list->head; (i++) & (L - 1))
        output[i] = list->list[i];

    list->head = list->tail = 0;
    return length;
}