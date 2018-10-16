#include "../wait.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <sys/types.h>
#include <sys/wait.h>
#endif

int
wait_pids_nohang(int const* pids, unsigned int len, int* wstat) {
#if WINDOWS_NATIVE
#else
  for(;;) {
    int w;
    int r = wait_nohang(&w);
    if(!r || (r == (int)-1)) return (int)r;
    {
      unsigned int i = 0;
      for(; i < len; i++)
        if(r == pids[i]) break;
      if(i < len) {
        *wstat = w;
        return 1 + i;
      }
    }
  }
  return -1;
#endif
}
