#include "../byte.h"
#include "../str.h"
#include <stdlib.h>

char*
str_dup(const char* s) {
  size_t n = str_len(s) + 1;
  char* r = malloc(n);
  if(r == NULL) return NULL;
  byte_copy(r, n, s);
  return r;
}
