//
// Created by egrsa on 26.10.2020.
//

#ifndef LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#define LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#include "../MSLFQ/MS_queue.h"
struct queue_with_id{
    lfqueue *lfqueue1;
    int id;
    HP *hp;
};
#endif //LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
