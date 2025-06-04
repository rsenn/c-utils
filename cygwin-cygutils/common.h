#ifndef CYGUTILS_COMMON_H
#define CYGUTILS_COMMON_H 1

#if defined(_WIN32) || defined(__WIN32) || defined(WIN32) || defined(__WIN32__)
#ifndef WIN32
#define WIN32 1
#endif
#ifndef _WIN32
#define _WIN32 1
#endif
#ifndef __WIN32
#define __WIN32 1
#endif
#ifndef __WIN32__
#define __WIN32__ 1
#endif
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */
#if STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#else
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#endif /*STDC_HEADERS*/

#if HAVE_STRING_H
#if !STDC_HEADERS && HAVE_MEMORY_H
#include <memory.h>
#endif
#include <string.h>
#elif HAVE_STRINGS_H
#include <strings.h>
#endif /*HAVE_STRING[S]_H*/

#if HAVE_STDARG_H
#include <stdarg.h>
#define VA_START(a, f) va_start(a, f)
#else
#if HAVE_VARARGS_H
#include <varargs.h>
#define VA_START(a, f) va_start(a)
#endif
#endif
#ifndef VA_START
error no variadic api
#endif /* VARIADIC */

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif /* TIME */

#if HAVE_UNISTD_H
#include <unistd.h>
#endif /*HAVE_UNISTD_H*/

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif /*HAVE_SYS_PARAM_H */

#if HAVE_ERRNO_H
#include <errno.h>
#endif /*HAVE_ERRNO_H*/

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif /*HAVE_FCNTL_H*/

#if HAVE_LIMITS_H
#include <limits.h>
#endif /*HAVE_LIMITS_H*/

#if HAVE_MALLOC_H
#include <malloc.h>
#endif /*HAVE_MALLOC_H*/

#if HAVE_CTYPE_H
#include <ctype.h>
#endif /*HAVE_CTYPE_H*/

#if HAVE_GETOPT_H
#include <getopt.h>
#endif /*HAVE_GETOPT_H*/

/* column needs TIOCGWINSZ */
#if HAVE_TERMIOS_H
#include <termios.h>
#endif

#if GWINSZ_IN_SYS_IOCTL
#include <sys/ioctl.h>
#endif
/* End: column needs TIOCGWINSZ */

#if HAVE_POPT_H
#include <popt.h>
#endif /*HAVE_POPT_H*/

#if HAVE_WINDOWS_H
#include <windows.h>
#else
#ifndef O_BINARY
#define O_BINARY 0
#endif
#define setmode(a, b)
#endif /*HAVE_WINDOWS_H*/

/* we don't include wchar.h or wctype.h here.  let widechar.h do that */

#if defined(_WIN32) && !defined(__CYGWIN__)

#define cygwin_conv_to_win32_path(a, b) strncpy(b, a, MAX_PATH)
#define cygwin_conv_to_full_win32_path(a, b) _fullpath(b, a, MAX_PATH)
#define cygwin_conv_to_full_posix_path(a, b) _fullpath(b, a, MAX_PATH)

#endif /* _WIN32 && !__CYGWIN__ */

#endif /* !CYGUTILS_COMMON_H */
