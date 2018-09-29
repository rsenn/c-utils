/* ISC license. */

#include <errno.h>
#include <sys/wait.h>

pid_t
wait_nointr(int* wstat) {
  pid_t r;
  do
    r = wait(wstat);
  while((r == (pid_t)-1) && (errno == EINTR));
  return r;
}
