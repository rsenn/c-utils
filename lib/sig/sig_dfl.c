#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>
#if !WINDOWS_NATIVE
#include <sys/signal.h>
#endif

#ifndef SIG_DFL
#define SIG_DFL ((void (*)(int))0)
#endif
#ifndef SIG_IGN
#define SIG_IGN ((void (*)(int))1)
#endif

/* sa_handler is the first member */
struct sigaction const sig_dfl = {SIG_DFL};
struct sigaction const sig_ign = {SIG_IGN};
