#include <stdlib.h>
#ifndef _MSC_VER
#include <stdint.h>
#endif

#include "memalign.h"

void*
memalign_alloc(size_t boundary, size_t size) {
  void** place = NULL;
  uintptr_t addr = 0;
  void* ptr = (void*)malloc(boundary + size + sizeof(uintptr_t));
  if(!ptr) return NULL;

  addr = ((uintptr_t)ptr + sizeof(uintptr_t) + boundary) & ~(boundary - 1);
  place = (void**)addr;
  place[-1] = ptr;

  return (void*)addr;
}

void
memalign_free(void* ptr) {
  void** p = NULL;
  if(!ptr) return;

  p = (void**)ptr;
  free(p[-1]);
}

void*
memalign_alloc_aligned(size_t size) {
#if defined(__x86_64__) || defined(__LP64) || defined(__IA64__) || defined(_M_X64) || defined(_WIN64)
  return memalign_alloc(64, size);
#elif defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(GEKKO)
  return memalign_alloc(32, size);
#else
  return memalign_alloc(32, size);
#endif
}

// void *memalign(size_t boundary, size_t size) __attribute__((weak,alias("memalign_alloc")));
