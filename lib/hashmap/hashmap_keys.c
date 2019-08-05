#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"
linked_list*
hashmap_keys(hashmap* map) {
  return map->keys;
}

