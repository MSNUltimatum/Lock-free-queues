//
// Created by egrsa on 26.10.2020.
//

#ifndef LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#define LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
#include "../LFS/LFStack.h"
struct queue_with_id{
    lfstack_t *lfstack;
    int id;
};
#endif //LOCK_FREE_QUEUE_QUEUE_WITH_ID_H
