#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/types.h>
#include <sys/wait.h>
#endif

int
wait_pids(int* pids, unsigned int n) {
  while(n) {
    int wstat;
    unsigned int i = 0;
    int pid = wait_nointr(&wstat);
    if(pid < 0)
      return 0;
    for(; i < n; i++)
      if(pid == pids[i])
        break;
    if(i < n)
      pids[i] = pids[--n];
  }
  return 1;
}
