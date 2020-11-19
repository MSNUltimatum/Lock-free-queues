//
// Created by Ultimatum on 25.10.2020.
//

#include "Consumer.h"
#include <stdio.h>
#include <zconf.h>
#include "../LFS/LFStack.h"
#include "../HelpStruct/queue_with_id.h"

void* consumer(void* arg){
    struct queue_with_id *stack = arg;
    int res = 0;
    sleep(5);
    do{
        res = (int) lfs_pop(stack->lfstack);
        if(res != 0) {
            printf("Consumer with id = %d, get %d\n", stack->id, res);
        }
    }while (1);
}