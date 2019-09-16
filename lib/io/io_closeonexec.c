#include "../windoze.h"

#if WINDOWS_NATIVE
#else
#endif

#include <fcntl.h>
#include <errno.h>
#include "../io_internal.h"

void
io_closeonexec(fd_t d) {
#if !(defined(_WIN32) || defined(_WIN64))
  fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) | FD_CLOEXEC);
#endif
}
