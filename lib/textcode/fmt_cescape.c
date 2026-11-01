#include "../fmt.h"
#include "../textcode.h"
#include "../str.h"

size_t
fmt_cescape(char* dest, const char* src, size_t len) {
  return fmt_cescape2(dest, src, len, "");
}
