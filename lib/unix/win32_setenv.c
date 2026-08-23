#include "../windoze.h"

#if WINDOWS_NATIVE

#include <windows.h>
#include <stdlib.h>
#include <errno.h>

/* mingw/MSVC don't provide POSIX setenv(3); wrap the CRT's _putenv_s()
 * (same call this library's own env_put2() already uses on WINDOWS_NATIVE,
 * see lib/env/env_put.c) rather than routing through this library's own
 * env_put family, since callers reaching for raw setenv() are usually
 * doing so specifically to bypass it. */
int
setenv(const char* name, const char* value, int overwrite) {
  if(!overwrite) {
    DWORD r = GetEnvironmentVariableA(name, 0, 0);

    if(r != 0 || GetLastError() != ERROR_ENVVAR_NOT_FOUND)
      return 0;
  }

  if(_putenv_s(name, value) != 0) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

#endif
