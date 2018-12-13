#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"
size_t
hashmap_size(hashmap* map) {
  return map->size;
}
