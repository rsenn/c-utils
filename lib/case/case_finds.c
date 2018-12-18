#include "../case.h"
#include "../str.h"

size_t
case_finds(const void* haystack, size_t hlen, const char* what) {
  return case_findb(haystack, hlen, what, str_len(what));
}
