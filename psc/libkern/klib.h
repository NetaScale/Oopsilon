#ifndef KLIB_H_
#define KLIB_H_

#include "nanoprintf.h"

#define elementsof(ARR) (sizeof(ARR) / sizeof(ARR[0]))

#define kprintf(...) npf_pprintf(md_kputc, NULL, __VA_ARGS__)
#define kvpprintf(...) npf_vpprintf(md_kputc, NULL, __VA_ARGS__)
#define ksnprintf(...) npf_snprintf(__VA_ARGS__)
#define kvsnprintf(...) npf_vsnprintf(__VA_ARGS__)
#define fatal(...)                                                         \
	{                                                                  \
		kprintf(__VA_ARGS__);                                      \
		while (1) {                                                \
			__asm__("cli");                                    \
		}                                                          \
	}
#define assert(...)                                                    \
	{                                                              \
		if (!(__VA_ARGS__))                                    \
			fatal("assertion failed: %s\n", #__VA_ARGS__); \
	}

#ifdef __cplusplus
extern "C" {
#endif

extern void md_kputc(int ch, void *ctx);

#ifdef __cplusplus
}
#endif

#endif /* KLIB_H_ */
