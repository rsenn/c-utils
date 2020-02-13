#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"
void
hashmap_free(hashmap* map) {
  size_t i;
  for(i = 0; i < map->capacity; i++) {
    if(map->table[i]) {
      linked_list_free(map->table[i]);
    }
  }
  linked_list_free(map->keys);
  alloc_free(map->table);
  alloc_free(map);
}
