/* ISC license. */

#include <signal.h>

struct sigaction const SKASIG_DFL = { SIG_DFL, 0 } ;
struct sigaction const SKASIG_IGN = { SIG_IGN, 0 } ;
