#include "../fmt.h"
#include "../textcode.h"
#include "../str.h"

static inline int
issafe(unsigned char c) {
  const char safe[] = "$/.=~-_";
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || safe[str_chr(safe, c)]);
}

size_t
fmt_urlencoded(char* dest, const char* src, size_t len) {
  return fmt_urlencoded2(dest, src, len, "");
}
