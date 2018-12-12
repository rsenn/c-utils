#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"
void
hashmap_put(hashmap* map, void* key, void* value) {
  linked_list* list;
  linked_list_node* head;
  hashmap_pair* pair;
  if(!(list = map->table[map->hash_func(key, map->capacity)])) {
    list = (linked_list*)safe_malloc(sizeof(linked_list));
    linked_list_init(list, (linked_list_destructor)safe_free);
    map->table[map->hash_func(key, map->capacity)] = list;
  }
  head = linked_list_head(list);
  while(head) {
    hashmap_pair* pair = (hashmap_pair*)head->data;
    if(map->comparator(pair->key, key) == 0) {
      pair->value = value;
      return;
    }
    head = head->next;
  }
  pair = (hashmap_pair*)safe_malloc(sizeof(hashmap_pair));
  pair->key = key;
  pair->value = value;
  linked_list_prepend(list, pair);
  linked_list_append(map->keys, key);
  map->size++;
}
