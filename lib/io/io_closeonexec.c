#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include <fcntl.h>
#include <errno.h>
#include "../io_internal.h"

void io_closeonexec(int64 d) {
#if !(defined(_WIN32) || defined(_WIN64))
  fcntl(d,F_SETFL,fcntl(d,F_GETFL,0) | FD_CLOEXEC);
#endif
}
