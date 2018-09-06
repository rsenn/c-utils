#include <stdarg.h>
#include "uint64.h"

void errmsg_puts(int64 fd, const char* s);
void errmsg_flush(int64 fd);
void errmsg_start(int64 fd);

void errmsg_write(int64 fd, const char* err, const char* message, va_list list);
