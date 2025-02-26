#include "../xml.h"
#include "../scan.h"

#include <ctype.h>

size_t
xml_scan_attrname(const char* x, size_t len, size_t* toklen) {
  size_t i, j = 0;

  if(isalpha(*x) || *x == '_') {
    j = i = scan_noncharsetnskip(x, "=\r\n\t />", len);

    if(i < len && x[i] == '=')
      j++;
    *toklen = i;
  }
  return j;
}
