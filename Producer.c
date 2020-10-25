//
// Created by Ultimatum on 25.10.2020.
//

#include "Producer.h"
#include "Queue.h"
#include <stdio.h>

void *producer(void *arg){
    lfqueue *queue = arg;
    int number = 1;
    for(int i = 0; i < 20; i++) {
        if (!enqueue(queue, number)) {
            continue;
        } else {
            printf("Producer insert %d\n", number);
            number += 1;
        }
    }
    return NULL;
}