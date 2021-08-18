#include "../windoze.h"
#include "../sig.h"

/* MT-unsafe */

#include <signal.h>

void
sig_restoreto(const sigset_t* ss, unsigned int n) {
  unsigned int i = 1;
  for(; i <= n; i++) {
    int h = sigismember(ss, i);
    if(h < 0)
      continue;
    if(h)
      sig_restore(i);
  }
}
