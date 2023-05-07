#include "../errmsg_internal.h"
#include "../str.h"
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>

void
errmsg_warnerr(int err, const char* message, ...) {
  va_list a;
  va_start(a, message);
  errmsg_write(2, strerror(err), message, a);
}
