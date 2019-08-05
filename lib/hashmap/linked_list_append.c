#include "../linked_list.h"
#include "../memory.h"

void
linked_list_append(linked_list* list, void* data) {
  linked_list_node *new_node, *node = list->head;

  while(node->next) {
    node = node->next;
  }
  new_node = safe_malloc(sizeof(linked_list_node));
  new_node->data = data;
  new_node->next = NULL;
  node->next = new_node;

  list->size++;
}

