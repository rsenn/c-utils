#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
linked_list*
hash_map_keys(hash_map* map) {
  return map->keys;
}
