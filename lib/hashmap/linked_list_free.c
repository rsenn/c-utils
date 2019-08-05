#include "../linked_list.h"
#include "../memory.h"

void
linked_list_free(linked_list* list) {
  linked_list_node* previous_node = list->head;
  linked_list_node* current_node = previous_node->next;

  while(current_node != NULL) {
    if(list->free_data != NULL) {
      list->free_data(current_node->data);
    }
    safe_free(previous_node);
    previous_node = current_node;
    current_node = previous_node->next;
  }

  safe_free(previous_node);

  list->head = NULL;

  safe_free(list);
}

