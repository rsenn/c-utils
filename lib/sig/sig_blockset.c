#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

int
sig_blockset(const void* set) {
#if !WINDOWS_NATIVE
  return sigprocmask(SIG_SETMASK, set, 0);
#else
  (void)set;
  return -1;
#endif
}
