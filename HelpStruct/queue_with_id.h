//
// Created by egrsa on 26.10.2020.
//

#ifndef LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#define LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#include "../BasketsLFQ/BasketsLFQ.h"
#include "HP.h"

struct queue_with_id{
     struct queue_t *lf_queue;
    int id;
    HP *hp;
    int* producerFinished;
    int poducerCount;
};
#endif //LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
