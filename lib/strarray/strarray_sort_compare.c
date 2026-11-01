#include "../strarray.h"
#include "../str.h"
#include <stdlib.h>
typedef int(cmp_fn_t)(const void*, const void*);

int
strarray_sort_compare(const char** a, const char** b) {
  return str_diff(*a, *b);
}
