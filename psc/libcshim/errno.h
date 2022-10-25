#ifndef ERRNO_H_
#define ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int errno;

#define EINVAL 1
#define EINTR 2
#define ENOMEM 3
#define ENOENT 4

#ifdef __cplusplus
}
#endif

#endif /* ERRNO_H_ */
