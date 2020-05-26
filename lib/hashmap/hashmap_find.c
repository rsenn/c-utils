#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"

hashmap_pair*
hashmap_find(hashmap* map, void* key) {
  linked_list_node* node = hashmap_search(map, key);
  return node ? node->data : NULL;
}
