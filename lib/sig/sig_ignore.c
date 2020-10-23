#include "../sig.h"
#include <signal.h>

int
sig_ignore(int sig) {
  struct sigaction sa;

  sigemptyset(&sa.sa_mask);
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  return sigaction(sig, &sa, 0);
}
