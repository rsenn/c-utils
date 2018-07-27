#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cb.h"

/* see http://cr.yp.to/cb.html */
struct critbit_node {
  void* child[2];
  size_t byte;
  unsigned int mask;
};

#define EXTERNAL_NODE 0
#define INTERNAL_NODE 1

static inline int
decode_pointer(void** ptr) {
  ptrdiff_t numvalue = (char*)*ptr - (char*)0;
  if(numvalue & 1) {
    *ptr = (void*)(numvalue - 1);
    return EXTERNAL_NODE;
  }
  return INTERNAL_NODE;
}

static inline void
from_external_node(void* ptr, void** key, size_t* keylen) {
  unsigned char* bytes = (unsigned char*)ptr;
#ifndef NDEBUG
  ptrdiff_t numvalue = bytes - (unsigned char*)0;
  assert(numvalue && (numvalue & 1) == 0);
  assert(key && keylen);
#endif
  memcpy(keylen, bytes, sizeof(size_t));
  *key = bytes + sizeof(size_t);
}
