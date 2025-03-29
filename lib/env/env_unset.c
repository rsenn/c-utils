#define _POSIX_SOURCE 1
#define _GNU_SOURCE 1
#define _DEFAULT_SOURCE 1
#include "../windoze.h"
#include "../env.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <stdlib.h>
#endif

int
env_unset(const char* name) {
#if WINDOWS_NATIVE
  return SetEnvironmentVariable(name, "");
#else
  unsetenv(name);
  return 1;
#endif
}
