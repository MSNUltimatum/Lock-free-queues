//
// Created by Ultimatum on 28.10.2020.
//

#include "Optimistic_lock_free_queue.h"
#include <malloc.h>
#include <stdatomic.h>
#include "../HelpStruct/exp_backoff.h"

#define CAS __sync_bool_compare_and_swap
#define QUEUE_EMPTY -1
#define DUMMY_VAL -2
#define CAS atomic_compare_exchange_weak

struct node *new_node(void *val);

struct pointer new_pointer(struct node *pNode, unsigned int i);

void fixList(struct queue *pQueue, struct pointer tail, struct pointer head);

void initQueue(struct queue *queue) {
    struct pointer dummy;
    dummy.ptr = new_node(NULL);
    dummy.tag = 0;
    queue->head = dummy;
    queue->tail = dummy;
}

void enqueueOpt(struct queue *q, void *val) {
    struct node *nd = new_node(val);
    struct pointer tail;
    struct pointer nextNd;
    while (1) {
        tail = q->tail;
        nextNd = new_pointer(tail.ptr, tail.tag + 1);
        nd->next = nextNd;
        if (CAS(&(q->tail), &tail, new_pointer(nd, tail.tag + 1))) {
            tail.ptr->prev = new_pointer(nd, tail.tag);
            break;
        }
        backoff(5, 1000, 2, 12500);
    }
}

void *dequeueOpt(struct queue *q) {
    struct pointer tail, head, nodePrev;
    struct node *dummy;
    void *val;
    while (1) {
        head = q->head;
        tail = q->tail;
        nodePrev = head.ptr->prev;
        val = head.ptr->value;
        if (head.ptr == q->head.ptr && head.tag == q->head.tag) {
            if ((int) val != DUMMY_VAL) {
                if (tail.ptr != head.ptr || tail.tag != head.tag) {
                    if (nodePrev.tag != head.tag) {
                        fixList(q, tail, head);
                        continue;
                    }
                } else {
                    dummy = new_node(DUMMY_VAL);
                    dummy->next = new_pointer(tail.ptr, tail.tag + 1);
                    if (CAS(&(q->tail), &tail, new_pointer(dummy, tail.tag + 1))) {
                        head.ptr->prev = new_pointer(dummy, tail.tag);
                    } else {
                        free(dummy);
                    }
                    continue;
                }
                if(CAS(&(q->head), &head, new_pointer(nodePrev.ptr, head.tag + 1))){
                    free(head.ptr);
                    return val;
                }
            } else {
                if(tail.ptr == head.ptr) {
                    return QUEUE_EMPTY;
                } else {
                    if(nodePrev.tag != head.tag){
                        fixList(q, tail, head);
                        continue;
                    }
                    CAS(&(q->head), &head, new_pointer(nodePrev.ptr, head.tag + 1));
                }
            }
        }
        backoff(5, 1000, 2, 12500);
    }
}

void fixList(struct queue *pQueue, struct pointer tail, struct pointer head) {
    struct pointer curNode, curNodeNext, nextNodePrev;
    curNode = tail;
    while ((head.ptr == pQueue->head.ptr && head.tag == pQueue->head.tag) &&
           (curNode.ptr != head.ptr || curNode.tag != head.tag)){
        curNodeNext = (curNode.ptr)->next;
        if(curNodeNext.tag != curNode.tag){
            return;
        }
        nextNodePrev = (curNodeNext.ptr)->prev;
        if(nextNodePrev.ptr != curNode.ptr || nextNodePrev.tag != curNode.tag - 1){
            curNodeNext.ptr->prev = new_pointer(curNode.ptr, curNode.tag - 1);
        }
        curNode = new_pointer(curNodeNext.ptr, curNode.tag - 1);
    }
}

struct pointer new_pointer(struct node *pNode, unsigned int i) {
    struct pointer p;
    p.ptr = pNode;
    p.tag = i;
    return p;
}

struct node *new_node(void *val) {
    struct node *nd = calloc(1, sizeof(struct node));
    nd->value = val;
    return nd;
}
