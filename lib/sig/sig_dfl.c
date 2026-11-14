#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>
#if !WINDOWS_NATIVE
#include <sys/signal.h>
#endif

#ifndef SIG_DFL
#define SIG_DFL 0l
#endif
#ifndef SIG_IGN
#define SIG_IGN 1l
#endif

/* sa_handler is the first member */
struct sigaction const sig_dfl = {(long)SIG_DFL};
struct sigaction const sig_ign = {(long)SIG_IGN};
