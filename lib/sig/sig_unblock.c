#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
typedef _sigset_t sigset_t;
#endif

/* block SIGINT and SIGCHILD before forking a child
 * ----------------------------------------------------------------------- */
void
sig_block(void) {
#if !WINDOWS_NATIVE
  static sigset_t oldset;
  sigset_t newset;

  sigemptyset(&newset);
  sigemptyset(&oldset);
  sigaddset(&newset, SIGINT);
#ifdef SIGCHLD
  sigaddset(&newset, SIGCHLD);
#endif
  sigprocmask(SIG_UNBLOCK, &newset, &oldset);
#endif
}
