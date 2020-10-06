#include "../path.h"

int
path_is_absolute_sa(const stralloc* p) {
  return path_is_absolute_b(p->s, p->len);
}
