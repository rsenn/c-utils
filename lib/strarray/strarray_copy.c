#include "../strarray.h"
#include "../str.h"

/* appends copies of all strings of src to dst (dst used to get one extra empty
 * element, and its old contents were overwritten); 1 on success, 0 when out of
 * memory */
int
strarray_copy(strarray* dst, const strarray* src) {
  char** s;

  strarray_foreach(src, s) {
    if(!strarray_push(dst, *s))
      return 0;
  }
  return 1;
}
