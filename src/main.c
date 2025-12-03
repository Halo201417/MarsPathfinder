#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/task.h"

int main(){

    printf("RT Project\n");
    printf("Requiered to be executed with sudo\n\n");

    pthread_t t_low = 0, t_med = 0, t_high = 0;
    pthread_attr_t attr;

    pthread_mutex_init(&resource_mutex, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    struct sched_param param;

    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t_high, &attr, task_high_priority, NULL);

    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t_med, &attr, task_medium_priority, NULL);

    pthread_join(t_low, NULL);
    pthread_join(t_med, NULL);
    pthread_join(t_high, NULL);

    pthread_mutex_destroy(&resource_mutex);
    pthread_attr_destroy(&attr);

    printf("Experiment finalized\n");

    return 0;
}