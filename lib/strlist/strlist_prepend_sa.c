#include "../byte.h"
#include "../strlist.h"

int
strlist_prepend_sa(strlist* sl, const stralloc* sa) {
  return strlist_prependb(sl, sa->s, sa->len);
}
