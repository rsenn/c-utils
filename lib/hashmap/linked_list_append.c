#include "../linked_list.h"
#include "../alloc.h"

void
linked_list_append(linked_list* list, void* data) {
  linked_list_node *new_node, *node = list->head;

  while(node->next) {
    node = node->next;
  }
  new_node = alloc(sizeof(linked_list_node));
  new_node->data = data;
  new_node->next = NULL;
  node->next = new_node;

  list->size++;
}
