#include "../dlist.h"
#include "../byte.h"

void
dlist_init(dlist* l) {
  byte_zero(l, sizeof(dlist));
}
