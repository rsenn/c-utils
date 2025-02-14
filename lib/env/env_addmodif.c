/* ISC license. */

#include "../env.h"
#include "../stralloc.h"
#include <sys/types.h>

int
env_addmodif(stralloc* sa, const char* s, const char* t) {
  size_t oldlen = sa->len;
  if(!s)
    return 1;
  if(!stralloc_cats(sa, s))
    return 0;
  if((t && (!stralloc_catb(sa, "=", 1) || !stralloc_cats(sa, t))) ||
     !stralloc_0(sa)) {
    sa->len = oldlen;
    return 0;
  }
  return 1;
}
