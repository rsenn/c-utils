#include "../sig.h"

#include <signal.h>

void
sig_blocknone(void) {
  sigset_t ss;
  sigemptyset(&ss);
  sigprocmask(SIG_SETMASK, &ss, 0);
}
