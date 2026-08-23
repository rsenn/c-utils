#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

/* MT-unsafe */

int
sig_push(int sig, sighandler_t_ref f) {
#if !WINDOWS_NATIVE
  struct sigaction ssa = {0};
  ssa.sa_handler = f;
  sigfillset(&ssa.sa_mask);
#ifdef SA_RESTART
  ssa.sa_flags = SA_RESTART;
#endif
  return sig_pusha(sig, &ssa);
#else
  (void)sig;
  (void)f;
  return -1;
#endif
}
