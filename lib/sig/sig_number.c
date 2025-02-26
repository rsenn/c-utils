#include "../sig.h"
#include "../case.h"
#include "sig-internal.h"

int
sig_number(char const* name) {
  sigtable_t const* p = sigtable;

  for(; p->name; p++)

    if(!case_diffs(name, p->name))
      break;
  return p->number;
}
