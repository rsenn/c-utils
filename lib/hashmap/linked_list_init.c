#include "../linked_list.h"
#include "../alloc.h"

void
linked_list_init(linked_list* list, linked_list_destructor free_data) {
  linked_list_node* sentinel = (linked_list_node*)alloc(sizeof(linked_list_node));
  sentinel->next = NULL;
  sentinel->data = NULL;
  list->head = sentinel;

  list->free_data = free_data;

  list->size = 0;
}
