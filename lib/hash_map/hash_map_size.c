#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
size_t
hash_map_size(hash_map* map) {
  return map->size;
}
