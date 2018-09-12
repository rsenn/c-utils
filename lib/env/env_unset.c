#include "../windoze.h"
#include "../env.h"

#if WINDOWS_NATIVE
#include <windows.h>
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

