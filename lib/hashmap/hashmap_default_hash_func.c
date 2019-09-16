#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"
size_t
hashmap_default_hash_func(const void* key, size_t capacity) {
  return *((size_t*)key) % capacity;
}
