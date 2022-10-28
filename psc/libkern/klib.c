/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/*
 * Copyright 2022 NetaScale Systems Ltd.
 * All rights reserved.
 */

#include <sys/time.h>

#include <kern/kmem.h>
#include <kern/liballoc.h>
#include <kern/sync.h>

#include <math.h>
#include <stdint.h>

#define NON_BUILTINS

#include <libkern/klib.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

spinlock_t lock_msgbuf = SPINLOCK_INITIALISER;
int __mlibc_errno;

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

FILE* stderr = (FILE*)-1;

/* ctype.h */
int isalpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int
isdigit(char c)
{
	if ((c >= '0') && (c <= '9'))
		return 1;
	return 0;
}

int
isspace(char c)
{
	if ((c == ' ') || (c <= '\t') || (c == '\n') || c == '\r' ||
	    c == '\v' || c == '\f')
		return 1;
	return 0;
}

int isupper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

int isxdigit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/* stdio.h */

size_t
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	for (size_t i = 0; i < size * nmemb; i++)
		md_kputc(*(char *)(ptr + i), NULL);
}

int
fprintf(FILE *stream, const char *format, ...)
{
	va_list ap;
	int	r;
	va_start(ap, format);
	r = kvpprintf(format, ap);
	va_end(ap);
	return r;
}

int
printf(const char *__restrict format, ...)
{
	va_list ap;
	int	r;
	va_start(ap, format);
	r = kvpprintf(format, ap);
	va_end(ap);
	return r;
}

int putchar(int c)
{
	md_kputc(c, NULL);
}

int sprintf(char *__restrict buffer, const char *__restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int result = kvsnprintf(buffer, INT32_MAX, format, args);
	va_end(args);
	return result;
}

int
snprintf(char *__restrict buffer, size_t max_size,
    const char *__restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int result = kvsnprintf(buffer, max_size, format, args);
	va_end(args);
	return result;
}

int vfprintf(FILE *__restrict stream, const char *__restrict format, __gnuc_va_list args)
{
	return kvpprintf(format, args);
}


int
vsnprintf(char *__restrict buffer, size_t	  max_size,
    const char *__restrict format, __gnuc_va_list args)
{
	return kvsnprintf(buffer, max_size, format, args);
}

/* stdlib.h */

/*
 * from muslibc
 */
int
atoi(const char *s)
{
	int n = 0, neg = 0;
	while (isspace(*s))
		s++;
	switch (*s) {
	case '-':
		neg = 1;
	case '+':
		s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit(*s))
		n = 10 * n - (*s++ - '0');
	return neg ? n : -n;
}

/*
 * strtod implementation.
 * author: Yasuhiro Matsumoto
 * license: public domain
 */

char	   *
skipwhite(q)
char *q;
{
	char *p = q;
	while (isspace(*p))
		++p;
	return p;
}

long double strtold(str, end) const char *str;
char **end;
{
	long double	    d = 0.0;
	int	    sign;
	int	    n = 0;
	const char *p, *a;

	a = p = str;
	p = skipwhite(p);

	/* decimal part */
	sign = 1;
	if (*p == '-') {
		sign = -1;
		++p;
	} else if (*p == '+')
		++p;
	if (isdigit(*p)) {
		d = (long double)(*p++ - '0');
		while (*p && isdigit(*p)) {
			d = d * 10.0 + (long double)(*p - '0');
			++p;
			++n;
		}
		a = p;
	} else if (*p != '.')
		goto done;
	d *= sign;

	/* fraction part */
	if (*p == '.') {
		long double f = 0.0;
		long double base = 0.1;
		++p;

		if (isdigit(*p)) {
			while (*p && isdigit(*p)) {
				f += base * (*p - '0');
				base /= 10.0;
				++p;
				++n;
			}
		}
		d += f * sign;
		a = p;
	}

	/* exponential part */
	if ((*p == 'E') || (*p == 'e')) {
		int e = 0;
		++p;

		sign = 1;
		if (*p == '-') {
			sign = -1;
			++p;
		} else if (*p == '+')
			++p;

		if (isdigit(*p)) {
			while (*p == '0')
				++p;
			if (*p == '\0')
				--p;
			e = (int)(*p++ - '0');
			while (*p && isdigit(*p)) {
				e = e * 10 + (int)(*p - '0');
				++p;
			}
			e *= sign;
		} else if (!isdigit(*(a - 1))) {
			a = str;
			goto done;
		} else if (*p == 0)
			goto done;

		if (d == 2.2250738585072011 && e == -308) {
			d = 0.0;
			a = p;
			goto done;
		}
		if (d == 2.2250738585072012 && e <= -308) {
			d *= 1.0e-308;
			a = p;
			goto done;
		}
		d *= pow(10.0, (long double)e);
		a = p;
	} else if (p > str && !isdigit(*(p - 1))) {
		a = str;
		goto done;
	}

done:
	if (end)
		*end = (char *)a;
	return d;
}

double strtod(const char *str,
char **end)
{
	return strtold(str, end);
}

float strtof(const char *str,
char **end)
{
	return strtold(str, end);
}

void *
malloc(size_t size)
{
	return liballoc_kmalloc(size);
}

void
free(void *ptr)
{
	return liballoc_kfree(ptr);
}

void *
calloc(size_t nmemb, size_t size)
{
	return liballoc_kcalloc(nmemb, size);
}

void *
realloc(void *ptr, size_t size)
{
	return liballoc_krealloc(ptr, size);
}

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	*memptr = malloc(size);
	return 0;
}

/* string.h */

void *
memchr(const void *src, int c, size_t n)
{
	const unsigned char *s = src;
	c = (unsigned char)c;
	for (; n && *s != c; s++, n--)
		;
	return n ? (void *)s : 0;
}

int
memcmp(const void *str1, const void *str2, size_t count)
{
	register const unsigned char *c1, *c2;

	c1 = (const unsigned char *)str1;
	c2 = (const unsigned char *)str2;

	while (count-- > 0) {
		if (*c1++ != *c2++)
			return c1[-1] < c2[-1] ? -1 : 1;
	}
	return 0;
}

void *
memcpy(void *restrict dstv, const void *restrict srcv, size_t len)
{
	unsigned char	    *dst = (unsigned char *)dstv;
	const unsigned char *src = (const unsigned char *)srcv;
	for (size_t i = 0; i < len; i++)
		dst[i] = src[i];
	return dst;
}

void *
memmove(void *dst, const void *src, size_t n)
{
	const char *f = src;
	char	   *t = dst;

	if (f < t) {
		f += n;
		t += n;
		while (n-- > 0)
			*--t = *--f;
	} else
		while (n-- > 0)
			*t++ = *f++;
	return dst;
}

void *
memset(void *b, int c, size_t len)
{
	char *ss = b;
	while (len-- > 0)
		*ss++ = c;
	return b;
}

char *
strcat(char *__restrict dest, const char *__restrict src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(const unsigned char *)s1 - *(const unsigned char *)--s2);
}

int strncmp(const char *a, const char *b, size_t max_size) {
	size_t i = 0;
	while(true) {
		if(!(i < max_size))
			return 0;
		unsigned char a_byte = a[i];
		unsigned char b_byte = b[i];
		if(!a_byte && !b_byte)
			return 0;
		// If only one char is null, one of the following cases applies.
		if(a_byte < b_byte)
			return -1;
		if(a_byte > b_byte)
			return 1;
		i++;
	}
}

char *
strchr(const char *s, int c)
{
	do {
		if (*s == c) {
			return (char *)s;
		}
	} while (*s++);
	return (0);
}

char *
strrchr(const char *s, int c)
{
	size_t length = strlen(s);
	for (size_t i = 0; i <= length; i++) {
		if (s[length - i] == c)
			return (char *)(s + (length - i));
	}
	return NULL;
}

char *
strcpy(char *restrict dst, const char *restrict src)
{
	while ((*dst++ = *src++))
		;
	return dst;
}

char *
strdup(const char *src)
{
	size_t size = strlen(src) + 1;
	char  *str = kmem_alloc(size);
	memcpy(str, src, size);
	return str;
}

size_t
strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		continue;
	return (s - str);
}

/* time.h */
int
gettimeofday(struct timeval *restrict tv, void *tz)
{
	tv->tv_sec = 0;
	tv->tv_usec = 0;
	return 0;
}

/* mlibc support */
void
abort(void)
{
	fatal("libc abort.");
}

void
__assert_fail(const char *assertion, const char *file, unsigned int line,
    const char *function)
{
	kprintf("In function %s, file %s:%d: Assertion '%s' failed!\n",
	    function, file, line, assertion);
	abort();
}


/* strtoll etc */

typedef long long llong_type;
typedef unsigned long long ullong_type;

long
strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

llong_type
strtoll(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register ullong_type acc;
	register int c;
	register ullong_type cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(ullong_type)LLONG_MIN : LLONG_MAX;
	cutlim = cutoff % (ullong_type)base;
	cutoff /= (ullong_type)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LLONG_MIN : LLONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

unsigned long
strtoul(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

ullong_type
strtoull(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register ullong_type acc;
	register int c;
	register ullong_type cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (ullong_type)ULLONG_MAX / (ullong_type)base;
	cutlim = (ullong_type)ULLONG_MAX % (ullong_type)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULLONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}
