#ifndef BOOL_H
#define BOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else

typedef enum {
 false = 0,
true = 1
} _Bool;

typedef _Bool bool;

#endif

#ifdef __cplusplus
}
#endif
#endif /* BOOL_H */
