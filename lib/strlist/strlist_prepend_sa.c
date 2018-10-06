#include "../strlist.h"
#include "../byte.h"

int
strlist_prepend_sa(strlist* sl, const stralloc* sa) {
  return strlist_prependb(sl, sa->s, sa->len);
}
