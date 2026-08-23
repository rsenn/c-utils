#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#include <signal.h>
#if !WINDOWS_NATIVE
#include <sys/signal.h>
#endif

struct sigaction const sig_dfl = {.sa_handler = SIG_DFL};
struct sigaction const sig_ign = {.sa_handler = SIG_IGN};
