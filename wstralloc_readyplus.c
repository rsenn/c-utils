#include "wstralloc.h"
#include <stdlib.h>

/* wstralloc_readyplus is like wstralloc_ready except that, if sa is
 * already allocated, wstralloc_readyplus adds the current length of sa
 * to len. */
int wstralloc_readyplus(wstralloc* sa, size_t len) {
  if(sa->s) {
    if(sa->len + len < len) return 0;  /* catch integer overflow */
    return wstralloc_ready(sa, sa->len + len);
  } else
    return wstralloc_ready(sa, len);
}
