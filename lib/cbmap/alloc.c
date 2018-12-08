#include "../windoze.h"


#include "alloc.h"

#include "../memalign.h"
#include "../byte.h"
#include "../str.h"
#include "../cbmap_internal.h"
#include <stdio.h>
#include <string.h>

#if 0 // defined(WINDOWS_NATIVE) && (_MSC_VER >= 0900)
#include <crtdbg.h>
#endif

#ifndef DEBUG
#define printf(args)
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
//#elif WINDOWS
#elif defined(HAVE_POSIX_MEMALIGN) || defined(__dietlibc__)
  a = posix_memalign(p, alignment, size);
#else
  a = !(*p = memalign_alloc(alignment, size));
//#else
//  a = !(*p = malloc(size));
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
  printf("%p     CBM_MEM_CALLOC %-20s (%03d): Allocated %g x %g = %g bytes\n", p, file, line, (float)count, (float)size, (float)(count * size));
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
  printf("%p     CBM_MEM_STRNDUP %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
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
  printf("%p     CBM_MEM_STRDUP %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
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
    byte_copy(result, size, p);
  }
#ifdef DEBUG
  printf("%p     CBM_MEM_DUP    %-20s (%03d): Allocated %g bytes\n", result, file, line, (float)size);
#endif
  return result;
}

void
cbmap_mem_debug_free(const void* ptr, const char* file, int line) {
  if(ptr != NULL) {
    CBM_DEBUG_FREES += 1;
    CBM_DEBUG_DEALLOCATED_BYTES += 0;
#ifdef DEBUG
    printf("%p --- CBM_MEM_FREE   %-20s (%03d): Deallocating\n", ptr, file, line);
#endif
    memalign_free((void*)ptr);
  }
}

int
cbmap_mem_posix_memalign(void** memptr, size_t alignment, size_t size, const char* file, int line) {
  int result;

#ifdef HAVE_ALIGNED_ALLOC
  result = !(*memptr = aligned_alloc(alignment, size));
#elif defined(HAVE__ALIGNED_MALLOC)
  result = !(*memptr = _aligned_malloc(size, alignment));
//#elif WINDOWS
#elif defined(HAVE_POSIX_MEMALIGN) || defined(__dietlibc__)
  result = posix_memalign(memptr, alignment, size);
#else
  result = !(*memptr = memalign_alloc(alignment, size));
//#else
//  result = !(*memptr = malloc(size));
#endif

  if(!result) {
    CBM_DEBUG_ALLOCATIONS += 1;
    CBM_DEBUG_ALLOCATED_BYTES += (unsigned int)size;
  }

#ifdef DEBUG
  printf("%p     CBM_MEM_ALIGNED %-20s (%03d): Allocated %g bytes\n", *memptr, file, line, (float)size);
#endif
  return result;
}

void
cbmap_mem_debug_mem(const char* file, int line) {
  unsigned int allocations = CBM_DEBUG_ALLOCATIONS;
  unsigned int deallocations = CBM_DEBUG_FREES;
  unsigned int check = allocations - deallocations;

#ifdef DEBUG
  printf("CBM_MEM_LOG %-20s (%d) ALLOCATIONS %d DEALLOCATIONS %d CHECK %d BYTES %d\n", file, line, allocations, deallocations, check,
          CBM_DEBUG_ALLOCATED_BYTES);
#endif
}
