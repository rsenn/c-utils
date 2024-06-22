#include "../io_internal.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#else
#endif

#include <fcntl.h>
#include <errno.h>

void
io_closeonexec(fd_type d) {
#if !(defined(_WIN32) || defined(_WIN64))
  fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) | FD_CLOEXEC);
#endif
}
