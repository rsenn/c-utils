#include "../str.h"
#include <string.h>

/* str_chr returns the index of the first needle in the string, or its length if
 * there is none (also when needle is the NUL, which ends the string) */
size_t
str_chr(const char* in, char needle) {
  const char* p = strchr(in, needle);

  return p ? (size_t)(p - in) : strlen(in);
}
