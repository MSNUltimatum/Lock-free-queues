#include <stdio.h>
#include "Queue.h"
#include <malloc.h>
#include "Producer.h"
#include "Consumer.h"
#include <pthread.h>


int main() {
    lfqueue *queue = calloc(1, sizeof(lfqueue));
    init(queue, 10);
    pthread_t threads[4];
    pthread_create(&threads[0], NULL, producer, queue);
    pthread_create(&threads[1], NULL, producer, queue);
    pthread_create(&threads[2], NULL, consumer, queue);
    pthread_create(&threads[3], NULL, consumer, queue);

    void* t;
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], t);
    }
    return 0;
}
