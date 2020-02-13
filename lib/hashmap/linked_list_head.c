#include "../linked_list.h"
#include "../alloc.h"

linked_list_node*
linked_list_head(linked_list* list) {
  return list->head->next;
}
