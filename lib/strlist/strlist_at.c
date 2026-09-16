#include "../strlist.h"

char*
strlist_at(const strlist* sl, size_t i) {
  size_t p, l = 0;

  if(sl->sa.s == 0)
    return 0;

  for(p = 0; p < sl->sa.len; ++p) {
    if(l == i)
      return &sl->sa.s[p];

    if(sl->sa.s[p] == sl->sep)
      ++l;
  }

  /* i one past the last item (no trailing separator to have counted
   * it) -- the one-past-the-end pointer, e.g. for strlist_range()'s
   * exclusive upper bound when a range extends to the list's end.
   * l is the separator count, i.e. (item count - 1), at this point. */
  if(l + 1 == i)
    return &sl->sa.s[sl->sa.len];

  return 0;
}
