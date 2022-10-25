#ifndef _TIME_H
#define _TIME_H

#include <sys/time.h>

// [7.27.1] Components of time

#define CLOCKS_PER_SEC ((clock_t)1000000)

#define TIME_UTC 1

// POSIX extensions.

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3
#define CLOCK_MONOTONIC_RAW 4
#define CLOCK_REALTIME_COARSE 5
#define CLOCK_MONOTONIC_COARSE 6
#define CLOCK_BOOTTIME 7

#ifdef __cplusplus
extern "C" {
#endif

struct timespec {
	time_t tv_sec;
	long tv_nsec;
};

// [7.27.1] Components of time

typedef long clock_t; // Matches Linux' ABI.

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	long int tm_gmtoff;
	const char *tm_zone;
};

// [7.27.2] Time manipulation functions

clock_t clock(void);
double difftime(time_t a, time_t b);
time_t mktime(struct tm *ptr);
time_t time(time_t *timer);
int timespec_get(struct timespec *ptr, int base);

// [7.27.3] Time conversion functions

char *asctime(const struct tm *ptr);
char *ctime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
struct tm *gmtime_r(const time_t *__restrict timer, struct tm *__restrict result);
struct tm *localtime(const time_t *timer);
size_t strftime(char *__restrict dest, size_t max_size,
		const char *__restrict format, const struct tm *__restrict ptr);

void tzset(void);

#ifdef __cplusplus
}
#endif


#endif // _TIME_H
