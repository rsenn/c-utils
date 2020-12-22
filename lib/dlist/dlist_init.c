#include "../list.h"
#include "../byte.h"

void
list_init(list* l) {
  byte_zero(l, sizeof(list));
}
