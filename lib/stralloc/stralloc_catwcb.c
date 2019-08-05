#include "../byte.h"
#include "../stralloc.h"
#include "../utf8.h"

/* stralloc_catb copies the string buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_catb leaves sa alone and returns 0. */
int
stralloc_catwcb(stralloc* sa, const wchar_t* buf, size_t len) {
  size_t i, n = 0;
  for(i = 0; buf[i]; ++i) n += wcu8len(buf[i]);

  if(stralloc_readyplus(sa, n)) {
    wcstou8s(&sa->s[sa->len], buf, len);
    sa->len += n;
    return 1;
  }
  return 0;
}

