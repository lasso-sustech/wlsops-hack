#ifndef __TIMING_USR_H__
#define __TIMING_USR_H__

#include <sys/time.h>

// #include <stdint.h>
// uint64_t rdtsc(){
//     unsigned int lo,hi;
//     __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
//     return ((uint64_t)hi << 32) | lo;
// }

typedef struct
{
    struct timeval marker[2];
    struct timeval result;
}timing_unit;

#define timing_start(x) \
    do { \
        gettimeofday(&x.marker[0], NULL); \
    } while(0)

#define timing_stop(x) \
    do { \
        gettimeofday(&x.marker[1], NULL); \
        timersub(&x.marker[1], &x.marker[0], &x.result); \
    } while(0)

#endif