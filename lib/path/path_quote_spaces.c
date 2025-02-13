#include "../path_internal.h"
#include "../utf8.h"

int
path_quote_spaces(stralloc* sa, const char* s) {
  size_t i;
  int has_spaces = 0;

  for(i = 0; s[i];) {
    size_t len = u8_len(&s[i], 1);
    wchar_t w;

    if(u8_to_wc(&w, &s[i]) < 1)
      return -1;

    if(w == ' ') {
      has_spaces = 1;
      break;
    }
  }

  stralloc_zero(sa);

  if(has_spaces)
    stralloc_catc(sa, '"');

  stralloc_cats(sa, s);

  if(has_spaces)
    stralloc_catc(sa, '"');

  stralloc_nul(sa);

  return has_spaces;
}
