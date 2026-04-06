#define _POSIX_C_SOURCE 199309L // Required to use CLOCK_MONOTONIC precision
#include <time.h>
#include <stdio.h>
#include "../include/utils.h"

/**
 * @brief Adds milliseconds to a timespec structure
 * Performs the conversion from milliseconds to nanoseconds to handles
 * the overflow to increment the seconds if necessary
 * @param ts Pointer to the original time structure
 * @param ms Milliseconds to add
 */
void add_ms(struct timespec *ts, long ms){
    // Convert milliseconds to nanoseconds and add them up
    ts->tv_nsec += ms * 1000000;

    // If nanoseconds exceed 1 second we adjust
    while(ts->tv_nsec >= 1000000000){
        ts->tv_nsec -= 1000000000;
        ts->tv_sec++;
    }
}

/**
 * @brief Accurately compares two timespec time structures
 * @param t1 Time 1
 * @param t2 Time 2
 * @return 1 (t1 > t2), -1 (t1 < t2) or 0 if identical
 */
int cmp_timespec(struct timespec t1, struct timespec t2){
    if(t1.tv_sec > t2.tv_sec){
        return 1;
    }

    if(t1.tv_sec < t2.tv_sec){
        return -1;
    }

    // If the seconds are equal we check the nanoseconds
    if(t1.tv_nsec > t2.tv_nsec){
        return 1;
    }

    if(t1.tv_nsec < t2.tv_nsec){
        return -1;
    }

    return 0;   // The are exactly equal
}

/**
 * @brief Calculates the elapsed difference in milliseconds between two times
 * @param start Initial timestamp
 * @param end Final timestamp
 * @return Total difference in milliseconds
 */
long diff_ms(struct timespec start, struct timespec end){
    long sec_diff = end.tv_sec - start.tv_sec;
    long nsec_diff = end.tv_nsec - start.tv_nsec;

    return (sec_diff * 1000) + (nsec_diff / 1000000000);
}

/**
 * @brief Load simulation function
 * Keeps the CPU constinuously evaluating a loop until the specified time 
 * passes. We use CLOCK_MONOTONIC to prevent system time jumps from ruining
 * the measurement
 * @param ms Milliseconds the CPU will remain locked
 */
void burn_cpu(long ms){
    struct timespec start, current, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Calculate the exact completion time
    end = start;
    add_ms(&end, ms);

    // Busy-wait loop, keeps the thread active on the CPU
    do{
        clock_gettime(CLOCK_MONOTONIC, &current);
    }while(cmp_timespec(current, end) < 0);
}

/**
 * @brief Visual load simulation function
 * Works the same as burn_cpu, but every 100 milliseconds it prints a
 * colored characted in the terminal. This creates a Gantt chart
 * @param ms Total duration of the task in milliseconds
 * @param color ANSI code to colorize the output
 * @param symbol Symbol to print for each 100 ms block
 */
void visual_burn_cpu(long ms, const char* color, const char* symbol){
    struct timespec start, current, end, next_tick;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Global completion time of the task
    end = start;
    add_ms(&end, ms);

    // Completion time for the next visual tick
    next_tick = start;
    add_ms(&next_tick, 100);

    do{
        clock_gettime(CLOCK_MONOTONIC, &current);

        // If we have surpassed the current tick time we print a block
        if(cmp_timespec(current, next_tick) >= 0){
            printf("%s%s\033[0m", color, symbol);
            fflush(stdout); // Force immediate console output
            add_ms(&next_tick, 100);    // Prepare the next tick
        }
    }while(cmp_timespec(current, end) < 0);
    printf("\n");   // Line break after finishing the whole task
}