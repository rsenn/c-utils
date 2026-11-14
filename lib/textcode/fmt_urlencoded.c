#include "../fmt.h"
#include "../textcode.h"
#include "../str.h"

size_t
fmt_urlencoded(char* dest, const char* src, size_t len) {
  return fmt_urlencoded2(dest, src, len, "");
}
