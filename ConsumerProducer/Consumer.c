//
// Created by Ultimatum on 25.10.2020.
//

#include <stdio.h>
#include "Consumer.h"
#include "../MSLFQ/MS_queue.h"
#include "../HelpStruct/queue_with_id.h"

void* consumer(void* arg){
    struct queue_with_id *queue = arg;
    struct hprectype *hprec = allocate_HPRec(queue->hp);
    int res;
    do{
        res = dequeue(queue->lf_queue, hprec, queue->hp);
        if (res == -1 && queue->poducerCount == *queue->producerFinished) {
            break;
        }
//        printf( "number = %d\n", res);
//        if(res != -1)
//            printf("id = %d, number = %d\n", queue->id, res);
    } while(1);
    return 0;
}