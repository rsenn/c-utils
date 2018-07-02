#include <sys/types.h>
#include "../list.h"

size_t
list_length(list *l) {
  link* link;
  size_t count = 0;
  
  for(link = l->head; link; link = link->next)
    ++count;

  return count;
}

