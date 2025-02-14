#include "../stralloc.h"
#include "../strarray.h"

int
strarray_push_sa(strarray* arr, const stralloc* sa) {
  return strarray_pushb(arr, sa->s, sa->len);
}

int
strarray_emplace_sa(strarray* arr, stralloc* sa) {
  char** d = array_allocate((array*)arr,
                            sizeof(char*),
                            array_length((array*)arr, sizeof(char*)));

  stralloc_nul(sa);

  return !!(*d = stralloc_moveb(sa, 0));
}
