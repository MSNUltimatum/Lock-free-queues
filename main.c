//
// Created by Ultimatum on 24.10.2020.
//
#include <stdio.h>
#include "MSLFQ/MS_queue.h"
#include <malloc.h>
#include "ConsumerProducer/Producer.h"
#include "ConsumerProducer/Consumer.h"
#include <pthread.h>
#include "HelpStruct/HP.h"
#include "HelpStruct/queue_with_id.h"
#define PRODUCER_COUNT 5
#define CONSUMER_COUNT 3


int main() {
    lfqueue *lfqueue = calloc(1, sizeof(lfqueue));
    HP* hp =  malloc(sizeof(HP));
    initMSqueue(lfqueue, 10);
    HP_init(hp);
    pthread_t threads[CONSUMER_COUNT + PRODUCER_COUNT];


    struct queue_with_id producerQueues[PRODUCER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producerQueues[i].lfqueue1 = lfqueue;
        producerQueues[i].id = i + 1;
        producerQueues[i].hp = hp;
    }

    struct queue_with_id consumerQueue[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumerQueue[i].lfqueue1 = lfqueue;
        consumerQueue[i].id = i + 1;
        consumerQueue[i].hp = hp;
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, producer, &producerQueues[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, consumer, &consumerQueue[i]);
    }

    void *t = NULL;
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
