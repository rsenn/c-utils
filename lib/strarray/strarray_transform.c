#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* appends pred(s) for every string s of in to out; pred returns an allocated
 * string that out then owns. Returns the number of strings handled. */
int
strarray_transform(strarray* in, strarray* out, char* (*pred)(const char*)) {
  size_t i, n = strarray_size(in);

  for(i = 0; i < n; ++i) {
    char* r = pred(strarray_AT(in, i));
    char** d;

    if(!r || !(d = (char**)array_allocate(&out->a, sizeof(char*), strarray_size(out))))
      return (int)i;
    *d = r;
  }
  return (int)i;
}
