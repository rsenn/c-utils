#include "../linked_list.h"
#include "../memory.h"
#include "../hash_map.h"

int
hash_map_contains_key(hash_map* map, void* key) {
  linked_list* list;
  linked_list_node* head;
  if(!(list = map->table[map->hash_func(key, map->capacity)]))
    return 0;
  head = linked_list_head(list);
  while(head) {
    hash_map_pair* pair = (hash_map_pair*)head->data;
    if(map->comparator(pair->key, key) == 0)
      return 1;
    head = head->next;
  }
  return 0;
}
