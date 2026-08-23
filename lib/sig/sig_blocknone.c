#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

int
sig_blocknone(void) {
#if !WINDOWS_NATIVE
  sigset_t ss;
  sigemptyset(&ss);
  return sigprocmask(SIG_SETMASK, &ss, 0);
#else
  return -1;
#endif
}
