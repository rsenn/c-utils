#include "strarray.h"

void
strarray_push(array* a, char* s) {
  array_catb(a, &s, sizeof(s)); 
}
