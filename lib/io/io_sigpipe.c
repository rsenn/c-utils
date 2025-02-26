#include "../io_internal.h"
#include <signal.h>
#include <sys/types.h>

/* this is an internal function, called by io_trywrite and io_waitwrite */

void
io_sigpipe(void) {
#if !(defined(_WIN32) || defined(_WIN64))
  static int isitdone;

  if(!isitdone) {
#if defined(SIGPIPE) && defined(SIG_IGN)
    signal(SIGPIPE, SIG_IGN);
    isitdone = 1;
#endif
  }
#endif
}
