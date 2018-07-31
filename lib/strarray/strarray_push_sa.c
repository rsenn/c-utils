#include "../strarray.h"
#include "../stralloc.h"

int
strarray_push_sa(strarray* arr, const stralloc* sa) {
  return strarray_pushb(arr, sa->s, sa->len);
}
