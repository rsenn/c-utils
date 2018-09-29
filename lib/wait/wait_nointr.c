#include "../windoze.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/wait.h>
#endif
#include <errno.h>

pid_t
wait_nointr(int* wstat) {
#if WINDOWS_NATIVE
  return -1;
#else
  pid_t r;
  do {
    r = wait(wstat);
  } while((r == (pid_t)-1) && (errno == EINTR));
  return r;
#endif
}
