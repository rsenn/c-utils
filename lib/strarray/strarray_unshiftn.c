#include "../alloc.h"
#include "../array.h"
#include "../byte.h"
#include "../str.h"
#include "../strarray.h"

/* inserts copies of the n strings of v at the front, in order; 1 on success */
int
strarray_unshiftn(strarray* arr, const char* const* v, size_t n) {
  return strarray_splice(arr, 0, 0, n, (const char**)v) != -1;
}
