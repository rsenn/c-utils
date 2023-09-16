#include "../stralloc.h"
#include "../fmt.h"
 
void
stralloc_escape_non_printable(stralloc* sa,const void*x,size_t n) {
  size_t i;
  const char* in=x;

  for(i = 0; i < n; ++i) {
    stralloc_readyplus(sa, 4);
    sa->len += fmt_escapecharnonprintable(&sa->s[sa->len], in[i]);
  }
}
