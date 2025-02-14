#include "../unix.h"
#include "../str.h"
#include "../strlist.h"

void
strlist_filter(const strlist* sl,
               strlist* matching,
               strlist* not_matching,
               const char* pattern) {
  const char* x;
  size_t n, plen = str_len(pattern);

  if(sl == not_matching) {
    strlist tmp;
    strlist_init(&tmp, sl->sep);

    strlist_filter(sl, matching, &tmp, pattern);
    strlist_free(not_matching);
    *not_matching = tmp;
    return;
  }

  if(sl == matching) {
    strlist tmp;
    strlist_init(&tmp, sl->sep);

    strlist_filter(sl, &tmp, not_matching, pattern);
    strlist_free(matching);
    *matching = tmp;
    return;
  }

  /* strlist_zero(matching);
   strlist_zero(not_matching);*/

  strlist_foreach(sl, x, n) {
    strlist* out = fnmatch_b(pattern, plen, x, n, FNM_CASEFOLD) == 0
                       ? matching
                       : not_matching;
    if(out)
      strlist_pushb(out, x, n);
  }
}
