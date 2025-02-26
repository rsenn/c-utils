#ifndef MAP_INTERNAL_H
#define MAP_INTERNAL_H

#include "bmap.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>

struct bmap_node_s {
  unsigned hash;
  void* value;
  bmap_node_t* next;
  /* char key[]; */
  /* char value[]; */
};

bmap_node_t** bmap_getref(bmap_base_t* m, const char* key);

static unsigned
bmap_hash(const char* str) {
  unsigned hash = 5381;

  while(*str) {
    hash = ((hash << 5) + hash) ^ *str++;
  }
  return hash;
}

static int
bmap_bucketidx(bmap_base_t* m, unsigned hash) {
  /* If the implementation is changed to allow a non-power-of-2 bucket
   * count, the line below should be changed to use mod instead of AND */
  return hash & (m->nbuckets - 1);
}
#endif
