//
// Created by Ultimatum on 20.11.2020.
//

#ifndef LOCK_FREE_QUEUE_HP_H
#define LOCK_FREE_QUEUE_HP_H

#include <stdbool.h>
#include "list.h"

struct hprectype {
    bool active;
    unsigned int rcount;
    struct listtype *rlist;
    void *HP[K];
    struct hprectype *next;
};

typedef struct _HP {
    struct hprectype *headHPRec;
    int H;
} HP;

int R(int H);

void scan(HP *hp, struct hprectype *myhprec);

void help_scan(HP *hp, struct hprectype *myhprec);

HP *HP_init();

struct hprectype *newHPRec();

struct hprectype *allocate_HPRec(HP *hp);

void retire_HPRec(struct hprectype *myhprec);

void retire_node(HP *hp, struct hprectype *myhprec, void *node);

#endif //LOCK_FREE_QUEUE_HP_H
