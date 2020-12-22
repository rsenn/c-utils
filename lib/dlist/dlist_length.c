#include <sys/types.h>
#include "../list.h"

size_t
dlist_length(list* l) {
  node* node;
  size_t count = 0;

  for(node = l->head; node; node = node->next) ++count;

  return count;
}
