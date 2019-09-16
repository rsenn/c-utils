#include <sys/types.h>
#include "../slist.h"

size_t
slist_length(slink** list) {
  slink* slink;
  size_t count = 0;

  for(slink = *list; slink; slink = slink->next) ++count;

  return count;
}
