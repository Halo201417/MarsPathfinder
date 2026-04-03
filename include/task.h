#ifndef TASKS_H
#define TASKS_H

#include <pthread.h>

/**
 * @brief Shared mutex used to protect the critical section.
 * In the failure mode, this mutex is the cause of the priority
 * inversion, in the fixed mode, this mutex is configured with the
 * Priority Inheritance Protocol
 */
extern pthread_mutex_t resource_mutex;

/**
 * @brief Low priority task routine (T_LOW)
 * Simulates the Mars Pathfinder meteorological task. Acquires the mutex
 * and holds the shared resource during its execution
 * @param arg Thread arguments
 * @return NULL
 */
void* task_low_priority(void* arg);

/**
 * @brief Medium priority task routine (T_MED)
 * Simulates the long-running communications task. It does not required
 * the mutex, but it preempts the low priority task and causes the CPU
 * hijack
 * @param arg Thread arguments
 * @return NULL
 */
void* task_medium_priority(void* arg);

/**
 * @brief High priority task routine (T_HIGH)
 * Simulates the critical information management bus task, attempts to
 * acquire the mutex locked by T_LOW and gets suspended, suffering the
 * unbounded priority inversion
 * @param arg Thread arguments
 * @return NULL
 */
void* task_high_priority(void* arg);

#endif