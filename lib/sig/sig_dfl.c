#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>
#if !WINDOWS_NATIVE
#include <sys/signal.h>
#endif

/* sa_handler is the first member */
struct sigaction const sig_dfl = {SIG_DFL};
struct sigaction const sig_ign = {SIG_IGN};
