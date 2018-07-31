#include "../strarray.h"

void
strarray_pushd(strarray* a, const char* s) {
  char* d = str_dup(s);
  array_catb(a, &d, sizeof(d));
}
d
