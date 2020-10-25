//
// Created by Ultimatum on 25.10.2020.
//
#include "Queue.h"
#include <malloc.h>
#include <stdio.h>
#include <stdatomic.h>
#define CAS __sync_bool_compare_and_swap


void init(lfqueue *lFqueue){
    node_q * dummy = malloc(sizeof(struct queue_node));
    lFqueue->head = dummy;
    lFqueue->tail = dummy;
}

int enqueue(struct LFqueue *lfqueue, void* data){
    struct queue_node *newNode = malloc(sizeof(node_q));
    newNode->data = data;
    newNode->next = NULL;
    while(1){
        struct queue_node *tail = lfqueue->tail;
        if(CAS(&tail->next, NULL, newNode)){
            CAS(&lfqueue->tail, tail, newNode);
            return 1;
        }
        else
        {
            CAS(&lfqueue->tail, tail, tail->next);
        }
    }
}



