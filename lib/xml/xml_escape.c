#include <ctype.h>
#include "../xml.h"
#include "../stralloc.h"
#include "../fmt.h"
#include "../byte.h"

static int
fmt_pred(int c) {
  if(isalpha(c) || isdigit(c) || c == ' ')
    return 0;
  return 1;
}

size_t
xml_escape(const char* s, size_t n, stralloc* out) {
  return stralloc_fmt_pred(out, s, n, fmt_xmlescape, &fmt_pred);
}
