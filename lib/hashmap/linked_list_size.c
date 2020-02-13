#include "../linked_list.h"
#include "../alloc.h"

size_t
linked_list_size(linked_list* list) {
  return list->size;
}
