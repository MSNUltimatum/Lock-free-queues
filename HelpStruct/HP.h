//
// Created by Ultimatum on 20.11.2020.
//

#ifndef LOCK_FREE_QUEUE_HP_H
#define LOCK_FREE_QUEUE_HP_H

#include "list.h"

struct hprec_t {
    int active;
    unsigned int rcount;
    struct list_t *rlist;
    void *HP[K];
    struct hprec_t *next;
};


typedef struct {
    struct hprec_t *headHPRec;
    int H;
} HP;

int R();

void scan(HP *hp, struct hprec_t *myhprec);

void help_scan(HP *hp, struct hprec_t *myhprec);

void HP_init(HP* hp);

struct hprec_t *newHPRec();

struct hprec_t *allocate_HPRec(HP *hp);

void retire_node(HP *hp, struct hprec_t *myhprec, void *node);

#endif //LOCK_FREE_QUEUE_HP_H
