#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"

void*
hashmap_get(hashmap* map, void* key) {
  linked_list_node* head = hashmap_search(map, key);
  hashmap_pair* pair = head ? head->data : NULL;
  return pair ? pair->value : NULL;
}
