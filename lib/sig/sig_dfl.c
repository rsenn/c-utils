#include "../sig.h"

#include <signal.h>

#undef sig_dfl
struct sigaction const sig_dfl = {
  {(void (*)(int))0}, 
  {0}
};
#undef sig_ign
struct sigaction const sig_ign = {
  {(void (*)(int))1}, 
  {0}
};
