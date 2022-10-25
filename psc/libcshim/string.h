#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NON_BUILTINS
void  *memchr(const void *s, int c, size_t n);
int    memcmp(const void *s1, const void *s2, size_t n);
void  *memcpy(void *dest, const void *src, size_t n);
void  *memmove(void *dest, const void *src, size_t n);
void  *memset(void *b, int c, size_t len);
size_t strlen(const char *str);
#else
#define memmove(dest, src, n) __builtin_memmove(dest, src, n)
#define memcpy(dest, src, n) __builtin_memcpy(dest, src, n)
#define memset(b, c, len) __builtin_memset(b, c, len)
#define strlen(str) __builtin_strlen(str)
#endif

char *strcat(char *restrict dest, const char *restrict src);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strcpy(char *dst, const char *src);

#ifdef __cplusplus
}
#endif

#endif /* STRING_H_ */
