#include "../buffer.h"
#include "../byte.h"
#include "../stralloc.h"
#include <errno.h>

int
buffer_gettok_sa(buffer* b, stralloc* sa, const char* charset, size_t setlen) {
  size_t i;

  buffer_freshen(b);

  for(i = 0;; ++i) {
    char c;
    if(buffer_peekc(b, &c) <= 0) return 0;
    if(byte_chr(charset, setlen, c) < setlen) break;
    if(!stralloc_readyplus(sa, 1)) goto nomem;
    stralloc_append(sa, &c);
    if(buffer_skipc(b) <= 0) return 0;
  }
  return i;

nomem:
  errno = ENOMEM;
  return -1;
}
