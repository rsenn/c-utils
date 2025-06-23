#include "../path_internal.h"

int
path_canonical_sa(stralloc* sa) {
  sa->len = path_normalize2(sa->s, sa->len);
  stralloc_nul(sa);
  return 1;
}
