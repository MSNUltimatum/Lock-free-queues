//
// Created by ultimatum on 26.11.2020.
//

#include <time.h>
#include "exp_backoff.h"

void backoff(int nInitial, int nBound, int nStep, long time){
    int nCurrent = nInitial;
    struct timespec tw = {0, time};
    struct timespec tr;
    for(int k = 0; k < nCurrent; k++){
        nanosleep(&tw, &tr);
    }

    nCurrent *= nStep;
    if(nCurrent > nBound)
        nCurrent = nBound;
}