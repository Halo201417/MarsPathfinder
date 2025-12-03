#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include "../include/task.h"

int main(int argc, char *argv[]){

    printf("RT Project\n");
    printf("Requiered to be executed with sudo\n\n");

    cpu_set_t my_set;
    CPU_ZERO(&my_set);
    CPU_SET(0, &my_set);

    if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set) == -1){
        perror("Error setting CPU affinity");
        return 1;
    }
    printf("SUCCESS: All tasks forced to run on CPU 0\n");

    struct sched_param main_param;
    
    main_param.sched_priority = 99;
    if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &main_param) != 0){
        perror("Error setting Main priority ('sudo' required)");
        return 1;
    }

    printf("SUCCESS: Main thread priority elevated to 99\n");

    int enable_fix = 0;
    if (argc > 1 && strcmp(argv[1], "fix") == 0){
        enable_fix = 1;
        printf("Mode: [ACTIVATE SOLUTION] Using Priority Inheritance\n");
    }
    else{
        printf("Mode: [EXPECTED FAILURE] Classical Priority Reversal\n");
    }
    
    printf("-----------------------------------------------------------\n");

    pthread_t t_low = 0, t_med = 0, t_high = 0;
    pthread_attr_t attr;

    if(enable_fix){
        pthread_mutexattr_t mutex_attr;

        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);

        pthread_mutex_init(&resource_mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
    }
    else{
        pthread_mutex_init(&resource_mutex, NULL);
    }

    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    struct sched_param param;

    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    if (pthread_create(&t_low, &attr, task_low_priority, NULL) != 0){
        perror("ERROR creating Low (Tip: Use 'sudo')");
        return 1;
    }

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