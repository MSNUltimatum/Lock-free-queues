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
#define PRODUCER_COUNT 4
#define CONSUMER_COUNT 1


int main() {
    lfqueue *lfqueue = calloc(1, sizeof(lfqueue));
    HP* hp =  malloc(sizeof(HP));
    initMSqueue(lfqueue, 10);
    HP_init(hp);
    pthread_t threads[CONSUMER_COUNT + PRODUCER_COUNT];
    int pf = 0;

    struct queue_with_id producerQueues[PRODUCER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producerQueues[i].lfqueue1 = lfqueue;
        producerQueues[i].id = i + 1;
        producerQueues[i].hp = hp;
        producerQueues[i].producerFinished = &pf;
    }

    struct queue_with_id consumerQueue[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumerQueue[i].lfqueue1 = lfqueue;
        consumerQueue[i].id = i + 1;
        consumerQueue[i].hp = hp;
        consumerQueue[i].producerFinished = &pf;
    }
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, producer, &producerQueues[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        pthread_create(&threads[PRODUCER_COUNT + i], NULL, consumer, &consumerQueue[i]);
    }

    time_t start, end;
    start = time(NULL);
    for (int i = 0; i < PRODUCER_COUNT + CONSUMER_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }
    end = time(NULL);
    printf("The interval was %.2f seconds", difftime(end, start));
    return 0;
}
