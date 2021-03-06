#include "../uint32.h"
#include "../hmap_internal.h"

int32
hmap_hash(const void* key, size_t len) {
  uint32 hmap_hash, i;

  for(hmap_hash = i = 0; i < len; ++i) {
    hmap_hash += ((const unsigned char*)key)[i];
    hmap_hash += (hmap_hash << 10);
    hmap_hash ^= (hmap_hash >> 6);
  }
  hmap_hash += (hmap_hash << 3);
  hmap_hash ^= (hmap_hash >> 11);
  hmap_hash += (hmap_hash << 15);
  return hmap_hash & 0x7FFFFFFF;
}
