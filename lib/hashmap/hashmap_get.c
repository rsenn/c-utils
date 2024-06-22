#include "../linked_list.h"
#include "../alloc.h"
#include "../hashmap.h"

void*
hashmap_get(hashmap* map, void* key) {
  linked_list_node* n = hashmap_search(map, key);

  hashmap_pair* p = n ? n->data : NULL;

  return p ? p->value : NULL;
}
