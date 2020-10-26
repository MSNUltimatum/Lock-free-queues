//
// Created by Ultimatum on 25.10.2020.
//
#include "Producer.h"
#include "Queue.h"
#include <stdio.h>
#include "queue_with_id.h"

void *producer(void *arg){
    struct queue_with_id *queue = arg;
    int number = 1;
    do {
        enqueue(queue->lfqueue1, (void *) number);
        printf("Producer with id = %d insert %d\n",queue->id, number);
        number += 1;
    }while (1);
    return NULL;
}