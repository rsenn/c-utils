#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

int
sig_block(int signum) {
#if !WINDOWS_NATIVE
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, signum);
  return sigprocmask(SIG_BLOCK, &ss, 0);
#else
  (void)signum;
  return -1;
#endif
}
