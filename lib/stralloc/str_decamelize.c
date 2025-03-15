#include "../str.h"
#include "../stralloc.h"

char*
str_decamelize(const char* s, char sep) {
  stralloc out;

  stralloc_init(&out);
  stralloc_decamelize(s, &out, sep);
  stralloc_nul(&out);

  return out.s;
}
