#include <assert.h>
#include <stdlib.h>
#include "byte.h"
#include "cb.h"

/* see http://cr.yp.to/cb.html */
struct critbit_node {
  void* child[2];
  size_t byte;
  unsigned int mask;
};

#define EXTERNAL_NODE 0
#define INTERNAL_NODE 1

static int
decode_pointer(void** ptr) {
  ptrdiff_t numvalue = (char*)*ptr - (char*)0;
  if(numvalue & 1) {
    *ptr = (void*)(numvalue - 1);
    return EXTERNAL_NODE;
  }
  return INTERNAL_NODE;
}

static void
from_external_node(void* ptr, void** key, size_t* keylen) {
  unsigned char* bytes = (unsigned char*)ptr;
#ifndef NDEBUG
  ptrdiff_t numvalue = bytes - (unsigned char*)0;
  assert(numvalue && (numvalue & 1) == 0);
  assert(key && keylen);
#endif
  byte_copy(keylen, sizeof(size_t), bytes);
  *key = bytes + sizeof(size_t);
}

static int
cb_less(const struct critbit_node* a, const struct critbit_node* b) {
  return a->byte < b->byte || (a->byte == b->byte && a->mask < b->mask);
}

static void*
make_external_node(const void* key, size_t keylen) {
  char* data = (char*)malloc(sizeof(size_t) + keylen);
#ifndef NDEBUG
  ptrdiff_t numvalue = (char*)data - (char*)0;
  assert((numvalue & 1) == 0);
#endif
  assert(keylen);
  byte_copy(data, sizeof(size_t), &keylen);
  byte_copy(data + sizeof(size_t), keylen, key);
  return (void*)(data + 1);
}

static struct critbit_node*
make_internal_node(void) {
  struct critbit_node* node = (struct critbit_node*)malloc(sizeof(struct critbit_node));
  return node;
}

static void
cb_free_node(void* ptr) {
  if(decode_pointer(&ptr) == INTERNAL_NODE) {
    struct critbit_node* node = (struct critbit_node*)ptr;
    cb_free_node(node->child[0]);
    cb_free_node(node->child[1]);
    free(node);
  } else {
    free(ptr);
  }
}
