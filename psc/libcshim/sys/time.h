#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

typedef int64_t suseconds_t;
typedef int64_t time_t;

struct timeval {
  time_t tv_sec;
  suseconds_t tv_usec;
};

#endif /* TIME_H_ */
