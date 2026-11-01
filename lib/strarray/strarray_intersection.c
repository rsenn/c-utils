#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* out gets copies of the strings of s1 that s2 contains, in the order of s1;
 * neither has to be sorted */
void
strarray_intersection(const strarray* s1, const strarray* s2, strarray* out) {
  char **p;

  strarray_foreach(s1, p) {
    if(strarray_index_of((strarray*)s2, *p) >= 0)
      strarray_push(out, *p);
  }
}
