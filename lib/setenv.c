#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
int
setenv(const char* name, const char* value, int overwrite) {
  return !SetEnvironmentVariable(name, value);
}
#endif
