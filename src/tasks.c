#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/task.h"
#include "../include/utils.h"

// Colors
#define C_LOW "\033[1;32m"  //Green
#define C_MED "\033[1;33m"  //Yellow
#define C_HIGH "\033[1;31m" //Red
#define C_RESET "\033[0m"

pthread_mutex_t resource_mutex;

void* task_low_priority(void* arg){
    printf("%s[LOW] Starting. Trying to obtain Mutex...%s\n", C_LOW, C_RESET);

    pthread_mutex_lock(&resource_mutex);
    printf("%s[LOW] Mutex BLOCK. Working on critical section (500ms)...%s\n", C_LOW, C_RESET);
    fflush(stdout);

    visual_burn_cpu(500, C_LOW, "█");

    printf("%s[LOW] Releasing Mutex.%s\n", C_LOW, C_RESET);
    pthread_mutex_unlock(&resource_mutex);

    printf("%s[LOW] Finished.%s\n", C_LOW, C_RESET);
    return NULL;
}

void* task_medium_priority(void* arg){
    usleep(200000);

    printf("\n%s[MED] Starting. I AM THE INTERFERENCE. Burning CPU (2000ms)...%s\n", C_MED, C_RESET);
    fflush(stdout);

    visual_burn_cpu(2000, C_MED, "█");

    printf("%s[MED] Finished.%s\n", C_MED, C_RESET);
    return NULL;
}

void* task_high_priority(void* arg){
    usleep(100000);

    printf("%s[HIGH] Starting. I need the Mutex URGENT...%s\n", C_HIGH, C_RESET);

    pthread_mutex_lock(&resource_mutex);

    printf("%s[HIGH] I finally obtain the Mutex (Fast critical section)%s\n", C_HIGH, C_RESET);
    fflush(stdout);

    visual_burn_cpu(100, C_HIGH, "█");

    pthread_mutex_unlock(&resource_mutex);

    printf("%s[HIGH] Finished.%s\n", C_HIGH, C_RESET);
    return NULL;
}