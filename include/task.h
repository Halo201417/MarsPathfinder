#ifndef TASKS_H
#define TASKS_H

#include <pthread.h>

extern pthread_mutex_t resource_mutex;

void* task_low_priority(void* arg);
void* task_medium_priority(void* arg);
void* task_high_priority(void* arg);

#endif