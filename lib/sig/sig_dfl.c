#define _POSIX_SOURCE 1
#include "../windoze.h"
#include "../sig.h"

#if !WINDOWS_NATIVE
#include <signal.h>
#include <sys/signal.h>
#endif

#undef sig_dfl
struct sigaction const sig_dfl = {.sa_handler = (void (*)(int))0
#if !defined(__MINGW32__) && !defined(__BORLANDC__) && !defined(_MSC_VER)
                                  ,
                                  .sa_mask = {{0}}
#endif
                                  ,
                                  .sa_flags = 0
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
                                  ,
                                  .sa_restorer = 0
#endif
};
#undef sig_ign
struct sigaction const sig_ign = {.sa_handler = (void (*)(int))1
#if !defined(__MINGW32__) && !defined(__BORLANDC__) && !defined(_MSC_VER)
                                  ,
                                  .sa_mask = {{0}}
#endif
                                  ,
                                  .sa_flags = 0
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MSYS__)
                                  ,
                                  .sa_restorer = 0
#endif
};
