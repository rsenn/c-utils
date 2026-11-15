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

/* a designated initializer where C99 has it: sa_handler is not the first
   member everywhere (bionic LP64 starts with sa_flags); old compilers
   (Borland C++) have no designators, there it is the first member */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define SIG_HANDLER_INIT(h) {.sa_handler = (h)}
#else
#define SIG_HANDLER_INIT(h) {(h)}
#endif

struct sigaction const sig_dfl = SIG_HANDLER_INIT(SIG_DFL);
struct sigaction const sig_ign = SIG_HANDLER_INIT(SIG_IGN);
