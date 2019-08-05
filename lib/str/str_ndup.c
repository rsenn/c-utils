#include "../byte.h"
#include "../str.h"
#include <stdlib.h>

char*
str_ndup(const char* s, size_t n) {
  char* r = malloc(n + 1);
  if(r == NULL) return NULL;
  byte_copy(r, n, s);
  r[n] = '\0';
  return r;
}
