#ifndef __TIMING_USR_H__
#define __TIMING_USR_H__

#include <linux/timekeeping.h>

// Taking from <sys/time.h>, timersub
# define timernsub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;			      \
    if ((result)->tv_nsec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_nsec += 1000000000;					      \
    }									      \
  } while (0)
#endif	/* Misc.  */

typedef struct
{
    struct timespec marker[2];
    struct timespec result;
}timing_unit;

#define timing_start(x) \
    do { \
        getnstimeofday(&x.marker[0]); \
    } while(0)

#define timing_stop(x) \
    do { \
        getnstimeofday(&x.marker[1]); \
        timernsub(&x.marker[1], &x.marker[0], &x.result) \
    } while(0)

#endif