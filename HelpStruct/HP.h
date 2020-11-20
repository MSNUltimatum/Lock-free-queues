//
// Created by Ultimatum on 20.11.2020.
//

#ifndef LOCK_FREE_QUEUE_HP_H
#define LOCK_FREE_QUEUE_HP_H

void init(int P, int K);

void RetireNode(void *node);

void Scan();

#endif //LOCK_FREE_QUEUE_HP_H
