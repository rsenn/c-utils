#include "../fmt.h"
#include "../textcode.h"
#include "../str.h"

size_t
fmt_quotedprintable(char* dest, const char* src, size_t len) {
  return fmt_quotedprintable2(dest, src, len, "");
}
