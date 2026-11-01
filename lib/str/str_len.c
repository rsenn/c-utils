#include "../str.h"
#include <string.h>

/* str_len returns the number of bytes before the terminating NUL */
size_t
str_len(const char* in) {
  return strlen(in);
}
