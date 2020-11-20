//
// Created by Ultimatum on 24.10.2020.
//
#include <stdio.h>
#include "LFS/LFStack.h"
#include <malloc.h>
#include "ConsumerProducer/Producer.h"
#include "ConsumerProducer/Consumer.h"
#include <pthread.h>
#include "HelpStruct/queue_with_id.h"
#define PRODUCER_COUNT 5
#define CONSUMER_COUNT 5


int main() {
    lfstack_t *lfstack = calloc(1, sizeof(lfstack_t));
    lfs_init(lfstack, 10);
    pthread_t threads[CONSUMER_COUNT + PRODUCER_COUNT];

    struct queue_with_id producerQueues[PRODUCER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producerQueues[i].lfstack = lfstack;
        producerQueues[i].id = i + 1;
    }

    struct queue_with_id consumerQueue[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumerQueue[i].lfstack = lfstack;
        consumerQueue[i].id = i + 1;
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, producer, &producerQueues[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, consumer, &consumerQueue[i]);
    }

    void *t = NULL;
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], t);
    }
    return 0;
}
