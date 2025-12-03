#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/task.h"
#include "../include/utils.h"

pthread_mutex_t resource_mutex;

void* task_low_priority(void* arg){
    printf("[LOW] Starting. Trying to obtain Mutex...\n");

    pthread_mutex_lock(&resource_mutex);
    printf("[LOW] Mutex BLOCK. Working on critical section (500ms)...\n");

    burn_cpu(500);

    printf("[LOW] Releasing Mutex.\n");
    pthread_mutex_unlock(&resource_mutex);

    printf("[LOW] Finished.\n");
    return NULL;
}

void* task_medium_priority(void* arg){
    usleep(200000);

    printf("[MED] Starting. I AM THE INTERFERENCE. Burning CPU (2000ms)...\n");

    burn_cpu(2000);

    printf("[MED] Finished.\n");
    return NULL;
}

void* task_high_priority(void* arg){
    usleep(100000);

    printf("[HIGH] Starting. I need the Mutex URGENT...\n");

    pthread_mutex_lock(&resource_mutex);

    printf("[HIGH] I finally obtain the Mutex (Fast critical section)\n");
    burn_cpu(100);
    pthread_mutex_unlock(&resource_mutex);

    printf("[HIGH] Finished.\n");
    return NULL;
}