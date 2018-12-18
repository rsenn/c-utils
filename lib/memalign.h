#ifndef MEMALIGN_H
#define MEMALIGN_H

#include <stdlib.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memalign_alloc_aligned(size_t size);
void* memalign_alloc(size_t boundary, size_t size);
void memalign_free(void* ptr);

#ifdef __cplusplus
}
#endif
#endif
