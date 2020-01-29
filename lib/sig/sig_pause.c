/* ISC license. */

#include <signal.h>

void sig_pause (void)
{
  sigset_t ss ;
  sigemptyset(&ss) ;
  sigsuspend(&ss) ;
}
