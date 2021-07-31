#include "../sig.h"

#include <signal.h>

#undef sig_dfl
struct sigaction const sig_dfl = {.sa_handler = (void (*)(int))0
#if !defined(__MINGW32__)
                                  , .sa_mask = {{0}}
#endif
                                  , .sa_flags = 0
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
                                  ,
                                  .sa_restorer = 0
#endif
};
#undef sig_ign
struct sigaction const sig_ign = {.sa_handler = (void (*)(int))1
#if !defined(__MINGW32__)
                                  , .sa_mask = {{0}}
#endif
                                  , .sa_flags = 0
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
                                  ,
                                  .sa_restorer = 0
#endif
};
