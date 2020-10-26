//
// Created by Ultimatum on 25.10.2020.
//

#include "Consumer.h"
#include <stdio.h>
#include "Queue.h"
#include "queue_with_id.h"

void* consumer(void* arg){
    struct queue_with_id *queue = arg;
    int res = 0;
    do{
        res = (int) dequeue(queue->lfqueue1);
        if(res != 0) {
            printf("Consumer with id = %d, get %d\n", queue->id, res);
        }
    }while (1);
}