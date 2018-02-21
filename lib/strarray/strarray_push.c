#include "strarray.h"

void
strarray_push(strarray* a, char* s) {
  array_catb(a, &s, sizeof(s)); 
}
