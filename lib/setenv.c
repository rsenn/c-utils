#if WINDOWS

#include <windows.h>

int
setenv(const char* name, const char* value, int overwrite) {
  return !SetEnvironmentVariable(name, value);
}
#endif
