#include "../xml.h"
#include "../str.h"
#include "../fmt.h"
#include "../byte.h"

void
xml_set_attribute_double(xmlnode* node, const char* a, double d, int prec) {
  char buf[64];
  size_t n = fmt_double(buf, d, sizeof(buf) - 1, prec);

  /* if(byte_chr(buf, n, '.') < n) {
    while(n > 0 && buf[n - 1] == '0') --n;
    if(n > 0 && buf[n - 1] == '.')
      --n;
  } */
  buf[n] = '\0';
  xml_set_attribute(node, a, buf);
}
