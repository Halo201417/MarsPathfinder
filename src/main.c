#define _GNU_SOURCE //Required to enable CPU affinity functinos like cpu_set_t
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

    // Environment configuration
    cpu_set_t my_set;
    CPU_ZERO(&my_set);
    CPU_SET(0, &my_set);    // We select exclusively Core 0

    /**
     * We force the program to run on a single core, if it ran on
     * multicore, the task could run in parallel and we wouldn't
     * see the problem or solution
     */
    if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set) == -1){
        perror("Error setting CPU affinity");
        return 1;
    }
    printf("SUCCESS: All tasks forced to run on CPU 0\n");

    // Main thread protection
    struct sched_param main_param; 
    main_param.sched_priority = 99; // The maximum priority allowed in POSIX RT

    /**
     * We elevate the main thread's priority to ensure the OS does not
     * interrupt it while it is creating and configuring the other threads
     */
    if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &main_param) != 0){
        perror("Error setting Main priority ('sudo' required)");
        return 1;
    }
    printf("SUCCESS: Main thread priority elevated to 99\n");

    // Argument parsing
    int enable_fix = 0;

    // If we pass the "fix" argument when executing
    if (argc > 1 && strcmp(argv[1], "fix") == 0){
        enable_fix = 1;
        printf("Mode: [ACTIVATE SOLUTION] Using Priority Inheritance\n");
    }
    else{
        printf("Mode: [EXPECTED FAILURE] Classical Priority Reversal\n");
    }
    
    printf("-----------------------------------------------------------\n");

    // Mutex initialization
    pthread_t t_low = 0, t_med = 0, t_high = 0;
    pthread_attr_t attr;

    if(enable_fix){
        // Solved mode: We configure the Mutex with the priority Inheritance Protocol
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);

        pthread_mutex_init(&resource_mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
    }
    else{
        // Failure mode: Standard Mutex, it allows priotiry inversion
        pthread_mutex_init(&resource_mutex, NULL);
    }

    // Thread scheduler configuration and task creation
    pthread_attr_init(&attr);
    // We tell pthreads to use our explicit attributes, not those inherited from the parent
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    // We set the strict real-time FIFO policy
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    struct sched_param param;

    // Low priority task creation
    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    if (pthread_create(&t_low, &attr, task_low_priority, NULL) != 0){
        perror("ERROR creating Low (Tip: Use 'sudo')");
        return 1;
    }

    // High priority task creation
    param.sched_priority = 30;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t_high, &attr, task_high_priority, NULL);

    // Medium priority task creation
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t_med, &attr, task_medium_priority, NULL);

    // Synchronization and cleanup
    // The main thread waits for all three tasks to finish their execution
    pthread_join(t_low, NULL);
    pthread_join(t_med, NULL);
    pthread_join(t_high, NULL);

    // We free memory from the OS structures
    pthread_mutex_destroy(&resource_mutex);
    pthread_attr_destroy(&attr);

    printf("Experiment finalized\n");

    return 0;
}