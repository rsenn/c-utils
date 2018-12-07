#include "../byte.h"
#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"

void
hash_map_init(hash_map* map, size_t capacity, hash_map_comparator comparator, hash_map_hash_func hash_func) {
  map->capacity = capacity;
  map->size = 0;
  map->table = (linked_list**)safe_malloc(sizeof(linked_list*) * map->capacity);
  byte_zero(map->table, sizeof(linked_list*) * map->capacity);
  if(comparator) {
    map->comparator = comparator;
  } else {
    map->comparator = hash_map_default_comparator;
  }
  if(hash_func) {
    map->hash_func = hash_func;
  } else {
    map->hash_func = hash_map_default_hash_func;
  }
  map->keys = (linked_list*)safe_malloc(sizeof(linked_list));
  linked_list_init(map->keys, NULL);
}
