#include "../path_internal.h"
#include "../utf8.h"
#include "../strlist.h"
#include <ctype.h>

int
path_split(const char* p, strlist* sl, int sep) {
  size_t i, j = 0;

  for(i = 0; p[i];) {
    wchar_t w;
    size_t len = u8_to_wc(&w, &p[i]);

    if(len >= 1 && w == sep) {
      strlist_pushb(sl, &p[j], i - j);
      j = i + len;
    }

    i += len;
  }

  if(i > j)
    strlist_pushb(sl, &p[j], i - j);

  return strlist_count(sl);
}
