#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/wait.h>
#endif

int
wait_ifexited(int wstat) {
#if WINDOWS_NATIVE
  /* wait_pid()/wait_pid_nohang() only report success once
   * GetExitCodeProcess() no longer reports STILL_ACTIVE (they filter that
   * case out themselves), so by the time a caller has *wstat the process
   * has always terminated normally in the sense this queries. */
  (void)wstat;
  return 1;
#else
  return WIFEXITED(wstat);
#endif
}
