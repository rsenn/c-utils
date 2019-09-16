#include "../byte.h"
#include "../str.h"

size_t
byte_finds(const void* haystack, size_t hlen, const char* what) {
  return byte_findb(haystack, hlen, what, str_len(what));
}
