#include "../env.h"

#if (defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__))
#include <windows.h>
#endif

int
env_unset(const char* name) {
#if (defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__))
  return SetEnvironmentVariable(name, "");
#else
  return unsetenv(name) == 0;
#endif
}

