#include "../windoze.h"
#include "../sig.h"

#include <signal.h>

int
sig_action(int sig, struct sigaction const* new, struct sigaction* old) {
#if !WINDOWS_NATIVE
  return sigaction(sig, new, old);
#else
  (void)sig;
  (void)new;
  (void)old;
  return -1;
#endif
}
