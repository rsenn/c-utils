#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"
void
hash_map_clear(hash_map* map) {
  size_t i;
  for(i = 0; i < map->capacity; i++) {
    linked_list* list = map->table[i];
    if(list) {
      linked_list_free(list);
      map->table[i] = NULL;
    }
  }
  map->size = 0;
}
