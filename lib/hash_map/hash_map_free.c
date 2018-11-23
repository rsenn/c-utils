#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
void
hash_map_free(hash_map* map) {
  size_t i;
  for(i = 0; i < map->capacity; i++) {
    if(map->table[i]) {
      linked_list_free(map->table[i]);
    }
  }
  linked_list_free(map->keys);
  safe_free(map->table);
  safe_free(map);
}
