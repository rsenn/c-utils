#include "../xml.h"
#include "../scan.h"
#include "../byte.h"

size_t
xml_scan_attrvalue(const char* x, size_t len, size_t* toklen) {
  size_t i, j;
  char quote[2] = {*x, '\0'};

  if(byte_chr("\r\n\t >/", 6, *x) == 6) {
    j = 0;
    i = 0;
  } else if(byte_chr("\"'", 2, *x) < 2) {

    for(i = 1; i < len; i++) {
      i += scan_noncharsetnskip(&x[i], quote, len - i);

      if(i > 0 && x[i - 1] != '\\')
        break;
    }
    j = ++i;
  }
  j += scan_whitenskip(x + j, len - j);

  *toklen = i;
  return j;
}
