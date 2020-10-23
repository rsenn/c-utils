#include "../str.h"

/* str_end returns 1 if the b is a suffix of a, 0 otherwise */
int
str_end(const char* a, const char* b) {
  return str_endb(a, b, str_len(b));
}
