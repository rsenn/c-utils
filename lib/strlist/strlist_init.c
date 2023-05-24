#include "../strlist.h"

void
strlist_init(strlist* sl, char sep) {
  stralloc_init(&sl->sa);
  sl->sep=sep;
}
