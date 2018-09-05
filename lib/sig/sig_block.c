#include "../sig.h"
#include <pthread.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
typedef _sigset_t sigset_t;
#endif

/* block SIGINT and SIGCHILD before forking a child
 * ----------------------------------------------------------------------- */
void
sig_block(void) {
  static sigset_t oldset;
  sigset_t newset;

  sigemptyset(&newset);
  sigemptyset(&oldset);
  sigaddset(&newset, SIGINT);
#ifdef SIGCHLD
  sigaddset(&newset, SIGCHLD);
#endif
  sigprocmask(SIG_BLOCK, &newset, &oldset);
}
