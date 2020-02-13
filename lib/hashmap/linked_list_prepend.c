#include "../linked_list.h"
#include "../memory.h"

void
linked_list_prepend(linked_list* list, void* data) {
  linked_list_node* new_node = alloc(sizeof(linked_list_node));
  new_node->data = data;
  new_node->next = list->head->next;
  list->head->next = new_node;

  list->size++;
}
