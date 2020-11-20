//
// Created by Ultimatum on 20.11.2020.
//

#include <stdlib.h>
#include "HP.h"

void* HP[K];
unsigned count = 0;
void* dlist[];

void RetireNode(void* node){
    dlist[count++] = node;

    if(count == R){
        Scan();
    }
}

void Scan(){
    unsigned i;
    unsigned p = 0;
    unsigned new_count = 0;

    void * hptr, plist[], new_dlist[];

    for (unsigned t=0; t < P; ++t) {
        void ** pHPThread = get_thread_data(t)->HP ;
        for (i = 0; i < K; ++i) {
            hptr = pHPThread[i];
            if ( hptr != nullptr )
                plist[p++] = hptr;
        }
    }

    sort(plist);

    for ( i = 0; i < R; ++i ) {
        if ( binary_search(dlist[i], plist))
            new_dlist[new_dcount++] = dlist[i];
        else
            free(dlist[i]);
    }

    // Stage 4 – формирование нового массива отложенных элементов.
    for (i = 0; i < new_count; ++i )
        dlist[i] = new_dlist[i];
    dcount = new_dcount;
}