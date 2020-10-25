//
// Created by Ultimatum on 25.10.2020.
//
#include "Queue.h"
#include <malloc.h>
#include <stdio.h>
#include <stdatomic.h>
#define CAS __sync_bool_compare_and_swap
#define QUEUE_OVERFLOW -2
#define QUEUE_EMPTY 0

void init(lfqueue *lfqueue1, int maxQueueSize){
    node_q * dummy = malloc(sizeof(struct queue_node));
    dummy->next = NULL;
    lfqueue1->head = dummy;
    lfqueue1->tail = dummy;
    lfqueue1->size = 0;
    lfqueue1->maxQueueSize = maxQueueSize;
}

int enqueue(struct LFqueue *lfqueue1, void* data){
    struct queue_node *tail, *newNode = calloc(1, sizeof(node_q));
    newNode->data = data;
    newNode->next = NULL;
    while (1) {
        if(lfqueue1->size < lfqueue1->maxQueueSize) {
            tail = lfqueue1->tail;
            if (CAS(&tail->next, NULL, newNode)) {
                CAS(&lfqueue1->tail, tail, newNode);
                lfqueue1->size += 1;
                return 1;
            } else {
                CAS(&lfqueue1->tail, tail, tail->next);
            }
        }
    }
}

void *dequeue(lfqueue *lfqueue1){
    while (1){
        node_q *head = lfqueue1->head;
        node_q *tail = lfqueue1->tail;
        node_q *nextHead = head->next;
        if(head == tail){
            if(nextHead == NULL){
                return (void *) QUEUE_EMPTY;
            }
            CAS(&lfqueue1->tail, head, nextHead);
        } else {
            void* result = nextHead->data;
            if(CAS(&lfqueue1->head, head, nextHead)) {
                free(head);
                lfqueue1->size -= 1;
                return result;
            }
        }
    }
}

void freeQueue(lfqueue *lfqueue1){
    node_q *head = lfqueue1->head;
    while (head->next != NULL){
        node_q *tmp = head;
        head = head->next;
        free(tmp);
    }
    free(head);
    free(lfqueue1->tail);
    free(lfqueue1);
}