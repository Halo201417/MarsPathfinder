#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/**
 * @brief Adds a specific number of milliseconds to a timespec structure,
 * useful for calculating deadlines or completion times
 * @param ts Pointer to the timespec structure to be modified
 * @param ms Milliseconds to add to the current time
 */
void add_ms(struct timespec *ts, long ms);

/**
 * @brief Compares two timespec time structures
 * @param t1 First time to compare
 * @param t2 Second time to compare
 * @return 1 if t1 is greater than t2
 * -1 if t1 is less than t2
 * 0 if both times are the same
 */
int cmp_timespec(struct timespec t1, struct timespec t2);

/**
 * @brief Calculates the time difference in milliseconds between two instants
 * @param start Start time
 * @param end End time
 * @return The elapsed difference in milliseconds (end - start)
 */
long diff_ms(struct timespec start, struct timespec end);

/**
 * @brief Simulates workload by keeping the CPU actively busy
 * Creates  a busy wait loop that consumes CPU cycles for the 
 * specified time
 * @param ms Milliseconds the CPU will be locked working
 */
void burn_cpu(long ms);

/**
 * @brief Visually simulates workload in the terminal
 * Prints colored blocks in real-time to demonstrate CPU usage,
 * making it easy to visualize execution and preemptions during presentations
 * @param ms Total milliseconds of CPU consumption
 * @param color ANSI color code for the printed text
 * @param symbol String to print for each time block
 */
void visual_burn_cpu(long ms, const char* color, const char* symbol);

#endif