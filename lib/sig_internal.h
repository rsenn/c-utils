#ifndef SIG_INTERNAL_H
#define SIG_INTERNAL_H

#include "sig.h"

#ifndef NSIG
#if 0 // def SIGRTMAX
#define NSIG (SIGRTMAX + 1)
#else
#define NSIG 65
#endif
#endif

typedef struct sigtable_s sigtable_t, *sigtable_t_ref;
struct sigtable_s {
  int number;
  char const* name;
};

extern sigtable_t const sigtable[];

#endif
