#include "../byte.h"
#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"

void*
hashmap_default_key_dup_func(const void* key) {
  return (void*)key;
}

void
hashmap_default_key_free_func(const void* pair) {}

void
hashmap_init(hashmap* map,
             size_t capacity,
             hashmap_comparator comparator,
             hashmap_hash_func hash_func,
             hashmap_key_dup_func key_dup_func,
             hashmap_key_free_func key_free_func) {
  map->capacity = capacity;
  map->size = 0;
  map->table =
      (linked_list**)alloc_zero(sizeof(linked_list*) * map->capacity);
  // byte_zero(map->table, sizeof(linked_list*) * map->capacity);
  if(comparator) {
    map->comparator = comparator;
  } else {
    map->comparator = hashmap_default_comparator;
  }
  if(hash_func) {
    map->hash_func = hash_func;
  } else {
    map->hash_func = hashmap_default_hash_func;
  }
  if(key_dup_func) {
    map->key_dup_func = key_dup_func;
  } else {
    map->key_dup_func = hashmap_default_key_dup_func;
  }
  if(key_free_func) {
    map->key_free_func = key_free_func;
  } else {
    map->key_free_func = hashmap_default_key_free_func;
  }
  // map->keys = (linked_list*)alloc(sizeof(linked_list));
  linked_list_init(&map->keys, map->key_free_func);
}
