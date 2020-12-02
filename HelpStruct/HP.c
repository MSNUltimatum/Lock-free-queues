//
// Created by Ultimatum on 20.11.2020.
//

#include <stdlib.h>
#include "HP.h"

void HP_init(HP *hp) {
    hp->H = 0;
    hp->headHPRec = NULL;
}

int R() {
    return MAXLENGTH / 2;
}

void scan(HP *hp, struct hprec_t *myhprec) {
    struct list_t *plist = list_init();
    struct hprec_t *hprec = hp->headHPRec;
    for (; hprec != NULL; hprec = hprec->next)
        for (int i = 0; i < K; i++)
            if (hprec->HP[i] != NULL)
                list_push(plist, hprec->HP[i]);

    sort(myhprec->rlist);

    void **tmplist = (void **) malloc(MAXLENGTH * sizeof(void *));
    int length = list_popAll(myhprec->rlist, tmplist);
    myhprec->rcount = 0;

    for (int i = 0; i < length; i++)
        if (list_lookup(plist, tmplist[i])) {
            list_push(myhprec->rlist, tmplist[i]);
            myhprec->rcount++;
        } else
            free(tmplist[i]);
    free(tmplist);
}

void help_scan(HP *hp, struct hprec_t *myhprec) {
    struct hprec_t *hprec = hp->headHPRec;
    for (; hprec; hprec = hprec->next) {
        if (hprec->active ||
            !__sync_bool_compare_and_swap(&hprec->active, 0, 1))
            continue;
        while (hprec->rcount > 0) {
            void *node = list_pop(hprec->rlist);
            hprec->rcount--;
            list_push(myhprec->rlist, node);
            myhprec->rcount++;
            if (myhprec->rcount >= R()) scan(hp, myhprec);
        }
        hprec->active = 0;
    }
}

struct hprec_t *newHPRec() {
    struct hprec_t *hprec = malloc(sizeof(struct hprec_t));
    hprec->active = 1;
    hprec->rcount = 0;
    for (int i = 0; i < K; i++)
        hprec->HP[i] = NULL;
    hprec->next = NULL;
    hprec->rlist = list_init();
    return hprec;
}

struct hprec_t *allocate_HPRec(HP *hp) {
    struct hprec_t *hprec = hp->headHPRec;
    for (; hprec; hprec = hprec->next)
        if (hprec->active ||
            !__sync_bool_compare_and_swap(&hprec->active, 0, 1))
            continue;
        else
            return hprec;

    __sync_fetch_and_add(&hp->H, K);

    struct hprec_t *myhprec = newHPRec();
    struct hprec_t *oldHead;
    do {
        oldHead = hp->headHPRec;
        myhprec->next = oldHead;
    } while (!__sync_bool_compare_and_swap(&hp->headHPRec, oldHead, myhprec));
    return myhprec;
}

void retire_node(HP *hp, struct hprec_t *myhprec, void *node) {
    for (int i = 0; i < K; i++)
        if (myhprec->HP[i] == node) {
            list_push(myhprec->rlist, node);
            myhprec->rcount++;
            myhprec->HP[i] = NULL;
            if (myhprec->rcount >= R()) {
                scan(hp, myhprec);
                help_scan(hp, myhprec);
            }
            break;
        }
}