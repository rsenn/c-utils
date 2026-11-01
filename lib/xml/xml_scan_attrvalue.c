#include "../xml.h"
#include "../scan.h"
#include "../byte.h"

/* scans an attribute value at x: a quoted string (the token includes both
 * quotes) or an unquoted run up to white space, '>' or '/'. *toklen is the
 * length of the token, the return value is that plus the white space after
 * it; 0 and a token length of 0 when there is no value. */
size_t
xml_scan_attrvalue(const char* x, size_t len, size_t* toklen) {
  size_t i = 0, j = 0;

  if(len > 0) {
    if(byte_chr("\"'", 2, *x) < 2) {
      char quote[2];

      quote[0] = *x;
      quote[1] = '\0';
      for(i = 1; i < len; i++) {
        i += scan_noncharsetnskip(&x[i], quote, len - i);

        if(i > 0 && x[i - 1] != '\\')
          break;
      }
      if(i < len)
        ++i; /* the closing quote */
      else
        i = len; /* not closed: the rest */
      j = i;
    } else if(byte_chr("\r\n\t >/", 6, *x) == 6) {
      j = i = scan_noncharsetnskip(x, "\r\n\t >/", len);
    }
    j += scan_whitenskip(x + j, len - j);
  }

  *toklen = i;
  return j;
}
