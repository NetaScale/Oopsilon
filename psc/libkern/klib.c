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

#define NON_BUILTINS

#include <libkern/klib.h>

#include <stdio.h>
#include <string.h>

spinlock_t lock_msgbuf = SPINLOCK_INITIALISER;

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

/* ctype.h */
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

/* stdio.h */
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

size_t
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	for (size_t i = 0; i < size * nmemb; i++)
		md_kputc(*(char *)(ptr + i), NULL);
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

double strtod(str, end) const char *str;
char **end;
{
	double	    d = 0.0;
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
		d = (double)(*p++ - '0');
		while (*p && isdigit(*p)) {
			d = d * 10.0 + (double)(*p - '0');
			++p;
			++n;
		}
		a = p;
	} else if (*p != '.')
		goto done;
	d *= sign;

	/* fraction part */
	if (*p == '.') {
		double f = 0.0;
		double base = 0.1;
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
		d *= pow(10.0, (double)e);
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
