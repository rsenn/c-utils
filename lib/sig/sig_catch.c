#include "../sig.h"

/* MT-unsafe */

int
sig_catch(int sig, sighandler_t_ref f) {
  struct sigaction ssa ={
    {f},
    {{SA_MASKALL | SA_NOCLDSTOP}},
    0,
    0
  };
  return sig_catcha(sig, &ssa);
}
