#include "../path_internal.h"
#include "../utf8.h"

int
path_quote_spaces_b(stralloc* sa, const char* x, size_t n) {
  size_t i = 0;
  int has_spaces = 0;

  while(i < n) {
    size_t len = u8len(&x[i], 1);
    wchar_t w;

    if(u8towc(&w, &x[i]) < 1)
      return -1;

    if(w == ' ') {
      has_spaces = 1;
      break;
    }
  }

  stralloc_zero(sa);

  if(has_spaces)
    stralloc_catc(sa, '"');

  stralloc_catb(sa, x, n);

  if(has_spaces)
    stralloc_catc(sa, '"');

  stralloc_nul(sa);

  return has_spaces;
}
