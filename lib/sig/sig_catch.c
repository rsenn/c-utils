#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>

#if !WINDOWS_NATIVE
#include <sys/signal.h>
#endif

/* MT-unsafe */

int
sig_catch(int sig, sighandler_t_ref f) {
#if !WINDOWS_NATIVE
  struct sigaction ssa = {
    .sa_handler = f
#if !defined(__MINGW32__) && !defined(__BORLANDC__) && !defined(_MSC_VER)
#ifdef __MSYS__
    ,
    .sa_mask = 0
#else
    ,
    .sa_mask = {0}
#endif
#endif
    ,
    .sa_flags = SA_MASKALL | SA_NOCLDSTOP
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
    ,
    .sa_restorer = 0
#endif

  };
  return sig_catcha(sig, &ssa);
#endif
  return -1;
}
