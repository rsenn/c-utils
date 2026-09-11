#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/wait.h>
#endif

int
wait_exitstatus(int wstat) {
#if WINDOWS_NATIVE
  /* on WINDOWS_NATIVE, wait_pid()/wait_pid_nohang() already store the raw
   * exit code from GetExitCodeProcess() in *wstat, not a packed status */
  return wstat;
#else
  return WEXITSTATUS(wstat);
#endif
}
