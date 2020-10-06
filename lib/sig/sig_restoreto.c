#include "../sig.h"

/* MT-unsafe */

#include <signal.h>

void
sig_restoreto(const void* set, unsigned int n) {
  unsigned int i = 1;
  for(; i <= n; i++) {
    int h = sigismember(set, i);
    if(h < 0)
      continue;
    if(h)
      sig_restore(i);
  }
}
