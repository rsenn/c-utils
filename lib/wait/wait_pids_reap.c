#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/types.h>
#include <sys/wait.h>
#endif

int
wait_pids_reap(int* pids, unsigned int len) {
  unsigned int n = 0;

  while(len) {
    int w;
    int r = wait_pids_nohang(pids, len, &w);

    if(r < 0)
      return r;
    else if(!r)
      break;
    pids[r - 1] = pids[--len];
    n++;
  }
  return n;
}
