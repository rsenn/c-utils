#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/types.h>
#include <sys/wait.h>
#endif

unsigned int
wait_reap() {
  unsigned int n = 0;
  int wstat;

  while(wait_nohang(&wstat) > 0)
    n++;
  return n;
}
