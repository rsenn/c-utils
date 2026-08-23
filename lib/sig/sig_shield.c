#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

int
sig_shield(void) {
#if !WINDOWS_NATIVE
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, SIGTERM);
  sigaddset(&ss, SIGQUIT);
  sigaddset(&ss, SIGABRT);
  sigaddset(&ss, SIGINT);
  sigaddset(&ss, SIGPIPE);
  sigaddset(&ss, SIGHUP);
  return sigprocmask(SIG_BLOCK, &ss, 0);
#else
  return -1;
#endif
}
