/* ISC license. */

#include <signal.h>

int sigsegv (void)
{
  return raise(SIGSEGV) == 0 ;
}
