#include "../list.h"
#include "../byte.h"

void
dlist_init(list* l) {
  byte_zero(l, sizeof(list));
}
