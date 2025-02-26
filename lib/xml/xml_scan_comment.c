#include "../xml.h"
#include "../byte.h"

size_t
xml_scan_comment(const char* x, size_t len) {
  size_t i = 0;

  if(len > 3 && byte_equal("<!--", 4, x)) {
    i = byte_finds(x, len, "-->");

    if(i + 3 <= len)
      i += 3;
  }
  return i;
}
