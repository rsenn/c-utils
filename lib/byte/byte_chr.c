#include "../byte.h"
#include <string.h>

/* byte_chr returns the smallest integer i between 0 and len - 1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t
byte_chr(const void* haystack, size_t len, char needle) {
  const char* p = len ? (const char*)memchr(haystack, (unsigned char)needle, len) : NULL;

  return p ? (size_t)(p - (const char*)haystack) : len;
}
