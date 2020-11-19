//
// Created by Ultimatum on 28.10.2020.
//

#include <crtdbg.h>
#include "Optimistic_lock_free_queue.h"
#include <malloc.h>
#include <stdatomic.h>
#define CAS __sync_bool_compare_and_swap
#define QUEUE_EMPTY NULL

struct node *new_node(void* val);

struct pointer new_pointer(struct node *pNode, unsigned int i);

void fixList(struct queue *pQueue, struct pointer tail, struct pointer head);

void initQueue(struct queue* queue){
    struct pointer dummy;
    dummy.ptr = new_node(NULL);
    dummy.tag = 0;
    queue->head = dummy;
    queue->tail = dummy;
}

void enqueueOpt(struct queue* q, void* val){
    struct node* nd = new_node(val);
    struct pointer tail;
    struct pointer nextNd;
    while (1){
        tail = q->tail;
        nextNd.ptr = tail.ptr;
        nextNd.tag = tail.tag + 1;
        nd->next = nextNd;
        if(atomic_compare_exchange_weak(&(q->tail), &tail, new_pointer(nd, tail.tag + 1))){
           tail.ptr->prev = new_pointer(nd, tail.tag);
           break;
        }
    }
}

void* dequeueOpt(struct queue* q){
    struct pointer tail, head, nodePrev;
    struct node* dummy;
    while (1){
        head = q->head;
        tail = q->tail;
        nodePrev = head.ptr->prev;
        if(head.ptr == q->head.ptr && head.tag == q->head.tag){
            if(tail.ptr != head.ptr || tail.tag != head.tag){
                if(nodePrev.tag != head.tag){
                    fixList(q, tail, head);
                    continue;
                }
                void * val = nodePrev.ptr->value;
                if(atomic_compare_exchange_weak(&(q->head), &head, new_pointer(nodePrev.ptr, head.tag + 1))){
                    free(head.ptr);
                    return val;
                }
            } else {
                return QUEUE_EMPTY;
            }
        }
    }
}

void fixList(struct queue *pQueue, struct pointer tail, struct pointer head) {
    struct pointer curNode;
    curNode = tail;
    while ((head.ptr == pQueue->head.ptr && head.tag == pQueue->head.tag) &&
          (curNode.ptr != head.ptr || curNode.tag != head.tag)){
        struct pointer currNodeNext = curNode.ptr->next;
        currNodeNext.ptr->prev = new_pointer(curNode.ptr, curNode.tag - 1);
        curNode = new_pointer(currNodeNext.ptr, curNode.tag - 1);
    }
}

struct pointer new_pointer(struct node *pNode, unsigned int i) {
    struct pointer p;
    p.ptr = pNode;
    p.tag = i;
    return p;
}

struct node *new_node(void* val) {
    struct node* nd = calloc(1, sizeof(struct node));
    nd->value = val;
    return nd;
}
