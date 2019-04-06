#ifndef ALLOC_H
#define ALLOC_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

void* alloc(size_t n);
void* alloc_zero(size_t n);
void alloc_free(void* x);

int alloc_re(void** x, size_t m, size_t n);

#ifdef __cplusplus
}
#endif

#endif
