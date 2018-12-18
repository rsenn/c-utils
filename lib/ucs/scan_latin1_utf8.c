#include "../ucs.h"
#include "../fmt.h"

size_t
scan_latin1_utf8(const char* x, size_t n, char* out) {
  uint32 wc;
  size_t ret;
  if((ret = scan_utf8(x, n, &wc)))
    *out = ucs_to_latin1_char(wc);
  return ret;
}
