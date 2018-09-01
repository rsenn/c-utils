#include <sys/types.h>
#include "../slist.h"

size_t
slist_length(slist *l) {
  slink* slink;
  size_t count = 0;

  for(slink = l->root; slink; slink = slink->next)
    ++count;

  return count;
}

