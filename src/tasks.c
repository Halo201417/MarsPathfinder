#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/task.h"
#include "../include/utils.h"

/**
 * Macros to colorize the terminal output and facilitate visual reading of
 * each task's behaviour during the live demonstration
 */
#define C_LOW "\033[1;32m"  // Green for the Low priority task
#define C_MED "\033[1;33m"  // Yellow for the Medium priority task
#define C_HIGH "\033[1;31m" // Red for the High priority task
#define C_RESET "\033[0m"   // Reset default terminal color


// Declared externally in task.h and initialized in main.c
 
pthread_mutex_t resource_mutex;

/**
 * @brief Low priority task (meteorological)
 * Starts at instant t=0, its objective is to acquire the mutex immediately
 * and perform a long task (500 ms)
 * - In failure mode: Suffers preemption from T_MED, holding the mutex for too long
 * - In solution mode: Inherits T_HIGH's priority and avoids interruption
 * @param arg Thread arguments
 * @return NULL
 */
void* task_low_priority(void* arg){
    printf("%s[LOW] Starting. Trying to obtain Mutex...%s\n", C_LOW, C_RESET);

    // Enter critical section
    pthread_mutex_lock(&resource_mutex);
    printf("%s[LOW] Mutex BLOCK. Working on critical section (500ms)...%s\n", C_LOW, C_RESET);
    fflush(stdout);

    // Simulates intensive processing that keeps the resource locked
    visual_burn_cpu(500, C_LOW, "█");

    printf("%s[LOW] Releasing Mutex.%s\n", C_LOW, C_RESET);
    // Exit critical section
    pthread_mutex_unlock(&resource_mutex);

    printf("%s[LOW] Finished.%s\n", C_LOW, C_RESET);
    return NULL;
}

/**
 * @brief Medium priority task (Communications)
 * Has a 200 ms delay, meaning it starts after T_LOW and T_HIGH. It does not need mutex.
 * Its only objective its to take over the CPU for 2000 ms, displacing T_LOW and
 * preventing it from releasing the mutex
 * @param arg Thread arguments
 * @return NULL
 */
void* task_medium_priority(void* arg){
    // Initial delay: simulates the task arriving at t=200ms
    usleep(200000);

    printf("\n%s[MED] Starting. I AM THE INTERFERENCE. Burning CPU (2000ms)...%s\n", C_MED, C_RESET);
    fflush(stdout);

    // CPU hijacking having a higher priority then T_LOW
    visual_burn_cpu(2000, C_MED, "█");

    printf("%s[MED] Finished.%s\n", C_MED, C_RESET);
    return NULL;
}

/**
 * @brief High priority task (Information management bus)
 * Has a 100 ms delay, starts when T_LOW alerady holds the mutex. After trying to
 * acquire it, it blocks immediately. It is the main victim of the Priority 
 * Inversion problem
 * @param arg Thread arguments
 * @return NULL
 */
void* task_high_priority(void* arg){
    // Initial delay: simulates the urgent task arriving at t=100ms
    usleep(100000);

    printf("%s[HIGH] Starting. I need the Mutex URGENT...%s\n", C_HIGH, C_RESET);

    // Attempts to acquire the Mutex. This is where it gets blocked
    // because T_LOW is holding it
    pthread_mutex_lock(&resource_mutex);

    printf("%s[HIGH] I finally obtain the Mutex (Fast critical section)%s\n", C_HIGH, C_RESET);
    fflush(stdout);

    // Fast critical work once it obtains the resource
    visual_burn_cpu(100, C_HIGH, "█");

    // Exit critical section
    pthread_mutex_unlock(&resource_mutex);

    printf("%s[HIGH] Finished.%s\n", C_HIGH, C_RESET);
    return NULL;
}