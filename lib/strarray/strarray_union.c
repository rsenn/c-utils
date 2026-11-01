#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* out gets copies of all strings of s1, then those of s2 that s1 does not
 * contain; both keep their order and neither has to be sorted. Linear search:
 * meant for the short lists of command lines and option sets. */
void
strarray_union(const strarray* s1, const strarray* s2, strarray* out) {
  char **p;

  strarray_foreach(s1, p) strarray_push(out, *p);
  strarray_foreach(s2, p) {
    if(strarray_index_of((strarray*)s1, *p) < 0)
      strarray_push(out, *p);
  }
}
