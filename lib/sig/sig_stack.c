#include "../sig.h"
/* ISC license. */

#include <errno.h>
#include <signal.h>

static struct sigaction sig_stack[NSIG - 1][SIGSTACKSIZE];
static unsigned int sigsp[NSIG - 1];

int
sig_pusha(int sig, struct sigaction const* ssa) {
  if((sig <= 0) || (sig >= NSIG))
    return (errno = EINVAL, -1);
  if(sigsp[sig - 1] >= SIGSTACKSIZE)
    return (errno = ENOBUFS, -1);
  if(sigaction(sig, ssa, &sig_stack[sig - 1][sigsp[sig - 1]]) == -1)
    return -1;
  return ++sigsp[sig - 1];
}

int
sig_pop(int sig) {
  if((sig <= 0) || (sig >= NSIG))
    return (errno = EINVAL, -1);
  if(!sigsp[sig - 1])
    return (errno = EFAULT, -1);
  if(sigaction(sig, &sig_stack[sig - 1][sigsp[sig - 1] - 1], 0) == -1)
    return -1;
  return --sigsp[sig - 1];
}
