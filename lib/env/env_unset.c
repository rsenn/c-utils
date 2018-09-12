#include "../env.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

int
env_unset(const char* name) {
#if WINDOWS_NATIVE
  return SetEnvironmentVariable(name, "");
#else
  return unsetenv(name) == 0;
#endif
}

