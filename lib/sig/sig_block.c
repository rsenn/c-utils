#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

void
sig_block(int signum) {
#if !WINDOWS_NATIVE
  sigset_t ss;
  sigemptyset(&ss);
  sigprocmask(SIG_SETMASK, 0, &ss);

  sigaddset(&ss, signum);
  sigprocmask(SIG_BLOCK, &ss, 0);
#endif
}
