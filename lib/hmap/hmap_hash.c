#include "../hmap_internal.h"

/* one-at-a-time hash of Bob Jenkins */
uint32
hmap_hash(const void* key, size_t len) {
  uint32 h = 0;
  size_t i;

  for(i = 0; i < len; ++i) {
    h += ((const unsigned char*)key)[i];
    h += (h << 10);
    h ^= (h >> 6);
  }

  h += (h << 3);
  h ^= (h >> 11);
  h += (h << 15);
  return h;
}
