#include "map.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>

struct map_node_t {
  unsigned hash;
  void* value;
  map_node_t* next;
  /* char key[]; */
  /* char value[]; */
};

map_node_t** map_getref(map_base_t* m, const char* key);

static unsigned
map_hash(const char* str) {
  unsigned hash = 5381;
  while(*str) {
    hash = ((hash << 5) + hash) ^ *str++;
  }
  return hash;
}

static int
map_bucketidx(map_base_t* m, unsigned hash) {
  /* If the implementation is changed to allow a non-power-of-2 bucket count,
   * the line below should be changed to use mod instead of AND */
  return hash & (m->nbuckets - 1);
}
