#ifndef STDLIB_H_
#define STDLIB_H_

#include <stddef.h>
#include <stdint.h>
#include <kern/liballoc.h>
#include <libkern/klib.h>

#define abort(...) fatal("LIBC ABORT")

void *alloca(size_t);

#define alloca __builtin_alloca

#define malloc liballoc_kmalloc
#define calloc liballoc_kcalloc
#define free liballoc_kfree
#define realloc liballoc_krealloc

#endif /* STDLIB_H_ */
