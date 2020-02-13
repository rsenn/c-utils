#include "../linked_list.h"
#include "../memory.h"
#include "../hashmap.h"

void
hashmap_remove(hashmap* map, void* key) {
  linked_list_node *previous_node, *current_node;
  size_t offset = map->hash_func(key, map->capacity);
  linked_list* list;
  if(!(list = map->table[offset]))
    return;
  previous_node = list->head;
  current_node = previous_node->next;
  for(;;) {
    if(map->comparator(((hashmap_pair*)current_node->data)->key, key) == 0) {
      previous_node->next = current_node->next;
      if(list->free_data) {
        list->free_data(current_node->data);
      }
      alloc_free(current_node);
      list->size--;
      map->size--;
      return;
    }
    if(current_node->next == NULL) {
      break;
    }
    previous_node = current_node;
    current_node = current_node->next;
  }
}
