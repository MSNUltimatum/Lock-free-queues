//
// Created by Ultimatum on 25.10.2020.
//
#include <stdio.h>
#include "Producer.h"
#include "../MSLFQ/MS_queue.h"
#include "../HelpStruct/queue_with_id.h"

void *producer(void *arg){
    struct queue_with_id *queue = arg;
    struct hprec_t *hprec = allocate_HPRec(queue->hp);
    int number = 1;
    for(int i = 1;i < 1000000; i++) {
        enqueue(queue->lfqueue1, (void *) i, hprec);
//        printf("Producer with id = %d insert %d\n",queue->id, number);
        number += 1;
    }
    int* a = queue->producerFinished;
    *a = *a + 1;
    *queue->producerFinished = *a;
    return 0;
}