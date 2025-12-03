#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include "../include/utils.h"

void add_ms(struct timespec *ts, long ms){
    ts->tv_nsec += ms * 1000000;

    while(ts->tv_nsec >= 1000000000){
        ts->tv_nsec -= 1000000000;
        ts->tv_nsec++;
    }
}

int cmp_timespec(struct timespec t1, struct timespec t2){
    if(t1.tv_sec > t2.tv_sec){
        return 1;
    }

    if(t1.tv_sec < t2.tv_sec){
        return -1;
    }

    if(t1.tv_nsec > t2.tv_nsec){
        return 1;
    }

    if(t1.tv_nsec < t2.tv_nsec){
        return -1;
    }

    return 0;
}

long diff_ms(struct timespec start, struct timespec end){
    long sec_diff = end.tv_sec - start.tv_sec;
    long nsec_diff = end.tv_nsec - start.tv_nsec;

    return (sec_diff * 1000) + (nsec_diff / 1000000000);
}

void burn_cpu(long ms){
    struct timespec start, current, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    end = start;
    add_ms(&end, ms);

    do{
        clock_gettime(CLOCK_MONOTONIC, &current);
    }while(cmp_timespec(current, end) < 0);
}