#include "../errmsg.h"
#include "../errmsg_internal.h"
#include "../str.h"
#include <stdarg.h>
#include <sys/types.h>

void
errmsg_warn(const char* message, ...) {
  va_list a;
  va_start(a, message);
  errmsg_write(2, 0, message, a);
}

