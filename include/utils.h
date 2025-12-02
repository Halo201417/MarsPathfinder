#ifndef UTILS_H
#define UTILS_H

#include <time.h>

void add_ms(struct timespec *ts, long ms);
int cmp_timespec(struct timespec t1, struct timespec t2);
long diff_ms(struct timespec start, struct timespec end);
void burn_cpu(long ms);

#endif