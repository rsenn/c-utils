#include "../linked_list.h"
#include "../hashmap.h"

linked_list_node*
hashmap_search(hashmap* map, void* key) {
  linked_list* list;
  linked_list_node* head;

  if(!(list = map->table[map->hash_func(key, map->capacity)]))
    return NULL;

  head = linked_list_head(list);

  while(head) {
    hashmap_pair* pair = (hashmap_pair*)head->data;

    if(map->comparator(pair->key, key) == 0)
      return head;

    head = head->next;
  }

  return NULL;
}
