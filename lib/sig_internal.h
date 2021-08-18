#ifndef SIG_INTERNAL_H
#define SIG_INTERNAL_H

#include "sig.h"

#define NSIG 31

typedef struct sigtable_s sigtable_t, *sigtable_t_ref;
struct sigtable_s {
  int number;
  char const* name;
};

extern sigtable_t const skalibs_sigtable[];

#endif
