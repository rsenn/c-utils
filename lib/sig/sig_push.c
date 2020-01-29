#include "../sig.h"

/* MT-unsafe */

int
sig_push(int sig, sighandler_t_ref f) {
  struct sigaction ssa;
  ssa.sa_handler = f;
  ssa.sa_mask.__val[0] = SA_MASKALL | SA_NOCLDSTOP;
  return sig_pusha(sig, &ssa);
}
