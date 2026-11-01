#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* inserts a copy of the first n bytes of b at the front; 1 on success */
int
strarray_unshiftb(strarray* arr, const char* b, size_t n) {
  char* d = str_ndup(b, n);
  int64 r;

  if(!d)
    return 0;
  r = strarray_splice(arr, 0, 0, 1, (const char**)&d);
  alloc_free(d); /* strarray_splice made its own copy */
  return r != -1;
}
