#include "../sig.h"

#include <signal.h>

void
sig_blockset(const void* set) {
  sigprocmask(SIG_SETMASK, set, 0);
}
