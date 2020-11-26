//
// Created by Ultimatum on 25.10.2020.
//

#include "Consumer.h"
#include <stdio.h>
#include "../MSLFQ/MS_queue.h"
#include "../HelpStruct/queue_with_id.h"

void* consumer(void* arg){
    struct queue_with_id *queue = arg;
    struct hprec_t *hprec = allocate_HPRec(queue->hp);
    int res = 0;
    do{
        res = (int) dequeue(queue->lfqueue1, hprec, queue->hp);
        if(res != 0) {
            printf("Consumer with id = %d, get %d\n", queue->id, res);
        }
    }while (1);
}