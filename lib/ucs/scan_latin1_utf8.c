#include "../ucs.h"
#include "../scan.h"


#include "../buffer.h"

size_t
scan_latin1_utf8(const char* x, size_t n, char* out) {
  uint32 wc;
  size_t ret;
  if((ret = scan_utf8(x, n, &wc))) {
    *out = ucs_to_latin1_char(wc);
    /*if(ret > 1 || wc > 0x7f) {
      buffer_puts(buffer_2, "UNICODE: 0x");
      buffer_putxlong0(buffer_2, wc, 4);
      buffer_puts(buffer_2, " LATIN1: '");
      buffer_putc(buffer_2, *out);
      buffer_putsflush(buffer_2, "'\n");
    }*/
  }
  return ret;
}
