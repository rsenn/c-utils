#include "../slist.h"

void
slist_removes(slink** p) {
  char* s = *(char**)slink_data(p);
  slink* l = slist_remove(p);
  free(s);
  alloc_free(l);
}
