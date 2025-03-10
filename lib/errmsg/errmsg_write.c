#include "../errmsg.h"
#include "../errmsg_internal.h"
#include <stdarg.h>

void
errmsg_write(int64 fd, const char* err, const char* message, va_list list) {
  errmsg_start(fd);
  errmsg_puts(fd, message);

  for(;;) {
    const char* s = va_arg(list, const char*);

    if(!s)
      break;
    errmsg_puts(fd, s);
  }
  va_end(list);

  if(err) {
    errmsg_puts(fd, ": ");
    errmsg_puts(fd, err);
  }
  errmsg_puts(fd, "\n");
  errmsg_flush(fd);
}
