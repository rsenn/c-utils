#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
size_t
hash_map_default_hash_func(const void* key, size_t capacity) {
  return *((size_t*)key) % capacity;
}
