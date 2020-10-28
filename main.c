//
// Created by Ultimatum on 24.10.2020.
//
#include <stdio.h>
#include "Optimistic_lock_free_queue.h"
#include <malloc.h>
#include "Producer.h"
#include "Consumer.h"
#include <pthread.h>
#include "queue_with_id.h"


#define PRODUCER_COUNT 5
#define CONSUMER_COUNT 5


int main() {
    struct queue *queue = calloc(1, sizeof(struct queue));
    initQueue(queue);
    pthread_t threads[CONSUMER_COUNT + PRODUCER_COUNT];

    struct queue_with_id producerQueues[PRODUCER_COUNT];
    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producerQueues[i].lfqueue1 = queue;
        producerQueues[i].id = i + 1;
    }

    struct queue_with_id consumerQueue[CONSUMER_COUNT];
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumerQueue[i].lfqueue1 = queue;
        consumerQueue[i].id = i + 1;
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, producer, &producerQueues[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        pthread_create(&threads[i], NULL, consumer, &consumerQueue[i]);
    }

    void* t;
    for (int i = 0; i < CONSUMER_COUNT + PRODUCER_COUNT; ++i) {
        pthread_join(threads[i], &t);
    }
    return 0;
}
