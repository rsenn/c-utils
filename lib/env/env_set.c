#include "../env.h"

#if (defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__))
#include <windows.h>
#endif

int
env_set(const char* name, const char* value) {
#if (defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__))
  return SetEnvironmentVariable(name, value);
#else
  return setenv(name, value, 1) == 0;
#endif
}

