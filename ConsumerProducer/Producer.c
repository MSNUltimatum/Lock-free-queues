//
// Created by Ultimatum on 25.10.2020.
//
#include "Producer.h"
#include "../LFS/LFStack.h"
#include <stdio.h>
#include <zconf.h>
#include "../HelpStruct/queue_with_id.h"

void *producer(void *arg){
    struct queue_with_id *stack = arg;
    int number = 1;
    for(int i = 1;i < 1000001; i++) {
        int FLAG = lfs_push(stack->lfstack, (void *) i);
        if( FLAG != 0)
            printf("FLAAAAG %d\n", FLAG);
        printf("Producer with id = %d insert %d\n",stack->id, number);
        number += 1;
    }
    return NULL;
}