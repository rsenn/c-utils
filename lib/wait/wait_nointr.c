#include "../wait.h"
#include "../windoze.h"
#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/wait.h>
#endif

#include <errno.h>

int
wait_nointr(int* wstat) {
#if WINDOWS_NATIVE
  return -1;
#else
  int r;
  do {
    r = wait(wstat);
  } while((r == (int) -1) && (errno == EINTR));
  return r;
#endif
}
