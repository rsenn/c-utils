#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

void
sig_unshield(void) {
#if !WINDOWS_NATIVE
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, SIGTERM);
  sigaddset(&ss, SIGQUIT);
  sigaddset(&ss, SIGABRT);
  sigaddset(&ss, SIGINT);
  sigaddset(&ss, SIGPIPE);
  sigaddset(&ss, SIGHUP);
  sigprocmask(SIG_UNBLOCK, &ss, 0);
#endif
}
