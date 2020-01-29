#include "../sig.h"
/* ISC license. */

#include <signal.h>

struct sigaction const SIG_DFL = {SIG_DFL, 0};
struct sigaction const SIG_IGN = {SIG_IGN, 0};
