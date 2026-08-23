#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>

/* MT-unsafe */

int
sig_catch(int sig, sighandler_t_ref f) {
#if !WINDOWS_NATIVE
  struct sigaction sa = {0};
  sa.sa_handler = f;
  sigfillset(&sa.sa_mask); /* block all signals while the handler runs */
#ifdef SA_RESTART
  sa.sa_flags = SA_RESTART;
#endif
  return sig_action(sig, &sa, 0);
#else
  (void)sig;
  (void)f;
  return -1;
#endif
}
