#ifndef _ALLOC_H
#define _ALLOC_H

#include <stdlib.h>

/*
   Set 'ALLOC_DEBUG' to 1 to enable poor man's memory debugging (not thread safe).
*/
#define ALLOC_DEBUG 0

#if ALLOC_DEBUG == 1

void* cbmap_mem_debug_calloc(size_t count, size_t size, const char* file, int line);
void cbmap_mem_debug_free(const void* ptr, const char* file, int line);
void cbmap_mem_debug_mem(const char* file, int line);
int cbmap_mem_posix_memalign(void** memptr, size_t alignment, size_t size, const char* file, int line);
char* cbmap_mem_debug_strdup(const char* p, const char* file, int line);
char* cbmap_mem_debug_strndup(const char* p, size_t size, const char* file, int line);
void* cbmap_mem_mem_dup(const void* p, size_t size, const char* file, int line);
int cbmap_mem_memalign(void** p, size_t alignment, size_t size);
double cbmap_mem_allocated(void);

#define CBM_CALLOC(n, s) cbmap_mem_debug_calloc((n), (s), __FILE__, __LINE__)
#define CBM_MALLOC(n) cbmap_mem_debug_calloc((n), 1, __FILE__, __LINE__)
#define CBM_FREE(p) cbmap_mem_debug_free((p), __FILE__, __LINE__)
#define CBM_MEM_LOG() cbmap_mem_debug_mem(__FILE__, __LINE__)
#define CBM_STRDUP(s) cbmap_mem_debug_strdup((s), __FILE__, __LINE__)
#define CBM_STRNDUP(s, n) cbmap_mem_debug_strndup((s), (n), __FILE__, __LINE__)
#define CBM_MEMDUP(k, v) cbmap_mem_mem_dup(k, v, __FILE__, __LINE__)
#define CBM_MEMALIGN(p, a, s) cbmap_mem_posix_memalign((p), (a), (s), __FILE__, __LINE__)
#define CBM_MEM_ALLOCATED() cbmap_mem_allocated()

#else
int cbmap_mem_memalign(void** p, size_t alignment, size_t size);

#include "../str.h"

#define CBM_CALLOC(n, s) calloc((n), (s))
#define CBM_MALLOC(n) malloc((n))
#define CBM_FREE(p) free((p))
#define CBM_STRDUP(s) str_dup((s))
#define CBM_STRNDUP(s, n) str_ndup((s), (n))
#define CBM_MEM_LOG()
#define CBM_MEMALIGN(p, a, s) cbmap_mem_memalign((p), (a), (s))
#define CBM_MEM_ALLOCATED() ((double)0)

#endif

#endif
