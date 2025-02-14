#include "../str.h"
#include "../strlist.h"

void
strlist_fromsq(strlist* sl,
               const char* s,
               const char* delim,
               const char* quote) {
  size_t ndelim = str_len(delim), nquote = str_len(quote);
  const char* x = s;
  size_t n;
  int q;
  if(sl->sa.len)
    stralloc_catc(&sl->sa, sl->sep);
  while(*x) {
    if(byte_chr(quote, nquote, *x) < nquote) {
      x++;
      n = str_chrs(x, quote, nquote);
      q = 1;
    } else {
      n = str_chrs(x, delim, ndelim);
      q = 0;
    }

    if(n) {
      strlist_pushb(sl, x, n);
    }
    x += n;

    if(q && *x && byte_chr(quote, nquote, *x) < nquote)
      x++;
    if(*x)
      x++;
  }
}
