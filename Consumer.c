//
// Created by Ultimatum on 25.10.2020.
//

#include "Consumer.h"
#include <stdio.h>
#include "Queue.h"

void* consumer(void* arg){
    lfqueue *queue = arg;
    int res = 0;
    do{
        res = dequeue(queue);
        if(res != 0){
            printf("Consumer get %d\n", res);
        }
    }while (res != 0);
}