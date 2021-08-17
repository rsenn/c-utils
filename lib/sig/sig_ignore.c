#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

int
sig_ignore(int sig) {
#if !WINDOWS_NATIVE
  struct sigaction sa;

  sigemptyset(&sa.sa_mask);
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  return sigaction(sig, &sa, 0);
#endif
}
