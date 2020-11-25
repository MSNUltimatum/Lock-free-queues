//
// Created by Ultimatum on 20.11.2020.
//

#include <stdlib.h>
#include "list.h"

struct list_t *list_init() {
    struct list_t *list = malloc(sizeof(struct list_t));
    list->head = list->tail = 0;
    for (int i = 0; i < MAXLENGTH; i++)
        list->list[i] = NULL;
    return list;
}


int list_push(struct list_t *list, void *node) {
    int position = list->tail + 1;
    if (position != MAXLENGTH) {
        list->list[list->tail] = node;
        list->tail = position;
        return position;
    } else
        return -1;
}

void *list_pop(struct list_t *list) {
    if (list->head != list->tail) {
        void *value = list->list[list->head];
        list->list[list->head] = NULL;
        list->head += 1;
        return value;
    } else {
        void *value = list->list[list->tail];
        list->list[list->tail] = NULL;
        return value;
    }
}

int list_lookup(struct list_t *list, void *node) {
    int right = list->tail;
    int left = list->head;
    int mid = 0;
    while (1) {
        mid = (left + right) / 2;

        if (node < list->list[mid])
            right = mid - 1;
        else if (node > list->list[mid]) {
            left = mid + 1;
        } else {
            return mid;
        }

        if (left > right) {
            return -1;
        }
    }
}

int list_popAll(struct list_t *list, void **output) {
    int length = list->tail - list->head;
    for (int i = list->head; i < list->tail; i++)
        output[i] = list->list[i];

    list->head = list->tail = 0;
    return length;
}

void sortList(void* list[], int head, int tail) {
    if (head < tail) {
        int left = head, right = tail;
        void *middle = list[(left + right) / 2];
        do {
            while (list[left] < middle) left++;
            while (list[right] > middle) right--;
            if (left <= right) {
                void *tmp = list[left];
                list[left] = list[right];
                list[right] = tmp;
                left++;
                right--;
            }
        } while (left <= right);
        sortList(list, head, right);
        sortList(list, left, tail);
    }
}

void sort(struct list_t* list) {
    sortList(list->list, list->head, list->tail);
}
