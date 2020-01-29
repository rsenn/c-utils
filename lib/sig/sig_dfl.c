#include "../sig.h"

#include <signal.h>

#undef SIG_DFL
struct sigaction const SIG_DFL = {(void (*)(int))0, 0};
#undef SIG_IGN
struct sigaction const SIG_IGN = {(void (*)(int))1, 0};
