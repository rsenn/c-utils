#include "wstralloc.h"

/* wstralloc_append adds one byte in[0] to the end of the wstring stored
 * in sa. It is the same as wstralloc_catb(&sa, in, 1). */
int wstralloc_append(wstralloc* sa, const wchar_t* in) {
  if(wstralloc_readyplus(sa, 1)) {
    sa->s[sa->len] = *in;
    ++sa->len;
    return 1;
  }
  return 0;
}

