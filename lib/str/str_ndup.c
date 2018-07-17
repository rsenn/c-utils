#include <stdlib.h>
#include "../str.h"
#include "../byte.h"

char*
str_ndup(const char* s, size_t n) {
  char* r = malloc(n+1);
  if(r == NULL) return NULL;
  byte_copy(r, n+1, s);
  return r;
}
