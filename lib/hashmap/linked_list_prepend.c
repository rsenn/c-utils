#include "../linked_list.h"
#include "../alloc.h"

linked_list_node*
linked_list_prepend(linked_list* list, void* data) {
  linked_list_node* new_node =
      (linked_list_node*)alloc(sizeof(linked_list_node));
  new_node->data = data;
  new_node->next = list->head->next;
  list->head->next = new_node;

  list->size++;
  return new_node;
}
