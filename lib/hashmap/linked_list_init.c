#include "../linked_list.h"
#include "../memory.h"

void
linked_list_init(linked_list* list, linked_list_destructor free_data) {
  linked_list_node* sentinel = alloc(sizeof(linked_list_node));
  sentinel->next = NULL;
  list->head = sentinel;

  list->free_data = free_data;

  list->size = 0;
}
