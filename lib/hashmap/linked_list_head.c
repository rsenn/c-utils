#include "../linked_list.h"
#include "../memory.h"

linked_list_node*
linked_list_head(linked_list* list) {
  return list->head->next;
}
