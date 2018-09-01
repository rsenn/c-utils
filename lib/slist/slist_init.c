#include "../slist.h"
#include "../byte.h"

void
slist_init(slist* l) {
  byte_zero(l, sizeof(slist));
}
