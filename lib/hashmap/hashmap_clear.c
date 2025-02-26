#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"
void
hashmap_clear(hashmap* map) {
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
