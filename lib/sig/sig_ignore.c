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
  struct sigaction sa = {0};
  sa.sa_handler = SIG_IGN;
  return sigaction(sig, &sa, 0);
#else
  (void)sig;
  return -1;
#endif
}
