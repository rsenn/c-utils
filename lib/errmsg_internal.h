#include <stdarg.h>
#include "uint64.h"
#include "windoze.h"

#if WINDOWS_NATIVE
# ifdef _MSC_VER
#  define _CRT_INTERNAL_NONSTDC_NAMES 1
# endif
# include <io.h>
#  if !defined(__LCC__) && !defined(__MINGW32__)
#   define read _read
#   define write _write
#   define open _open
#   define close _close
#  endif
#else
# include <unistd.h>
#endif

void errmsg_puts(int64 fd, const char* s);
void errmsg_flush(int64 fd);
void errmsg_start(int64 fd);
void errmsg_write(int64 fd, const char* err, const char* message, va_list list);


#include "errmsg.h"
