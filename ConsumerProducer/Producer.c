//
// Created by Ultimatum on 25.10.2020.
//

#include "Producer.h"
#include "../HelpStruct/queue_with_id.h"
#include "../BasketsLFQ/BasketsLFQ.h"



void *producer(void *arg) {
    struct queue_with_id *queue = arg;
//     struct hprectype *hprec = allocate_HPRec(queue->hp);
    for (int i = 1; i < 10000000; i++) {
        baskets_enqueue(queue->lf_queue, i);
//        printf("Producer with id = %d insert %d\n",queue->id, number);
    }
    int *a = queue->producerFinished;
    *a = *a + 1;
    *queue->producerFinished = *a;
    return 0;
}