/* ISC license. */

#include <signal.h>

int sigfpe (void)
{
  return raise(SIGFPE) == 0 ;
}
