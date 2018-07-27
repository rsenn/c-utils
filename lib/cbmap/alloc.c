#include "../str.h"
#include "alloc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <crtdbg.h>
#endif

static unsigned int CBM_DEBUG_ALLOCATIONS = 0;
static unsigned int CBM_DEBUG_FREES = 0;
static unsigned int CBM_DEBUG_ALLOCATED_BYTES = 0;
static unsigned int CBM_DEBUG_DEALLOCATED_BYTES = 0;

double
cbmap_mem_allocated(void) {
  return CBM_DEBUG_ALLOCATED_BYTES;
}

int
cbmap_mem_memalign(void** p, size_t alignment, size_t size) {
  int a;

#ifdef HAVE_ALIGNED_ALLOC
  a = !(*p = aligned_alloc(alignment, size));
#elif defined(HAVE__ALIGNED_MALLOC)
  a = !(*p = _aligned_malloc(size, alignment));
//#elif defined(_WIN32) || defined(_WIN64)
//  a = !(*p = _aligned_malloc_dbg(size, alignment, __FILE__, __LINE__)); // This is reduced to a call to `_aligned_malloc` when _DEBUG is not defined
#elif HAVE_POSIX_MEMALIGN
  a = posix_memalign(p, alignment, size);
#else
  a = !(*p = malloc(size));
#endif

  if(!a) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += size;
  } 
  
  return a;
}

void*
cbmap_mem_debug_calloc(size_t count, size_t size, const char* file, int line) {
  void* p;

  p = calloc(count, size);
  if(p != NULL) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += ((unsigned int)count * (unsigned int)size);
  }
#ifdef DEBUG
  fprintf(stdout, "%p     CBM_MEM_CALLOC %-20s (%03d): Allocated %g x %g = %g bytes\n", p, file, line, (float)count, (float)size, (float)(count * size));
#endif
  return p;
}

char*
cbmap_mem_debug_strndup(const char* p, size_t size, const char* file, int line) {
  char* result = str_ndup(p, size);
  if(result != NULL) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += size;
  }
#ifdef DEBUG
  fprintf(stdout, "%p     CBM_MEM_STRNDUP %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
#endif
  return result;
}

char*
cbmap_mem_debug_strdup(const char* p, const char* file, int line) {
  char* result;
  size_t size = 0;

  if(p == NULL)
    return NULL;

  result = str_dup(p);
  if(result != NULL) {
    CBM_DEBUG_ALLOCATIONS += 1;
    size = str_len(p);
    CBM_DEBUG_ALLOCATED_BYTES += size;
  }
#ifdef DEBUG
  fprintf(stdout, "%p     CBM_MEM_STRDUP %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
#endif
  return result;
}

void*
cbmap_mem_mem_dup(const void* p, size_t size, const char* file, int line) {
  void* result;

  if(p == NULL)
    return NULL;

  result = malloc(size);
  if(result != NULL) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += size;
    memcpy(result, p, size);
  }
#ifdef DEBUG
  fprintf(stdout, "%p     CBM_MEM_DUP    %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
#endif
  return result;
}

void
cbmap_mem_debug_free(const void* ptr, const char* file, int line) {
  if(ptr != NULL) {
    CBM_DEBUG_FREES += 1;
    CBM_DEBUG_DEALLOCATED_BYTES += 0;
#ifdef DEBUG
    fprintf(stdout, "%p --- CBM_MEM_FREE   %-20s (%03d): Deallocating\n", ptr, file, line);
#endif
    free((void*)ptr);
  }
}

int
cbmap_mem_posix_memalign(void** memptr, size_t alignment, size_t size, const char* file, int line) {
  int result;

#ifdef HAVE_ALIGNED_ALLOC
  result = !(*memptr = aligned_alloc(alignment, size));
#elif defined(HAVE__ALIGNED_MALLOC)
  result = !(*memptr = _aligned_malloc(size, alignment));
//#elif defined(_WIN32) || defined(_WIN64)
//  result = !(*memptr = _aligned_malloc_dbg(size, alignment, file, line)); // This is reduced to a call to `_aligned_malloc` when _DEBUG is not defined
#elif HAVE_POSIX_MEMALIGN
  result = posix_memalign(memptr, alignment, size);
#else
  result = !(*memptr = malloc(size));
#endif

  if(!result) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += (unsigned int)size;
  }
  
#ifdef DEBUG
  fprintf(stdout, "%p     CBM_MEM_ALIGNED %-20s (%03d): Allocated %g bytes\n", *memptr, file, line, (float)size);
#endif
  return result;
}

void
cbmap_mem_debug_mem(const char* file, int line) {
  unsigned int allocations = CBM_DEBUG_ALLOCATIONS;
  unsigned int deallocations = CBM_DEBUG_FREES;
  unsigned int check = allocations - deallocations;

  fprintf(stdout, "CBM_MEM_LOG %-20s (%d) ALLOCATIONS %d DEALLOCATIONS %d CHECK %d BYTES %d\n", file, line, allocations, deallocations, check,
          CBM_DEBUG_ALLOCATED_BYTES);
}
