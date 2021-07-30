#include "../sig.h"

/* MT-unsafe */

int
sig_catch(int sig, sighandler_t_ref f) {
  struct sigaction ssa = {.sa_handler = f, .sa_mask = {{0}}, .sa_flags = SA_MASKALL | SA_NOCLDSTOP
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
	  , .sa_restorer = 0
#endif

  };
  return sig_catcha(sig, &ssa);
}
