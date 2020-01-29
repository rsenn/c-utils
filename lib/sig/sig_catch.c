#include "../sig.h"

/* MT-unsafe */

int
sig_catch(int sig, sighandler_t_ref f) {
  struct sigaction ssa = {f, SA_MASKALL | SA_NOCLDSTOP};
  return sig_catcha(sig, &ssa);
}
