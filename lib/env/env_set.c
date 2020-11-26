#include "../windoze.h"

#include "../env.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

int
env_set(const char* name, const char* value) {
#if WINDOWS_NATIVE
  return SetEnvironmentVariable(name, value);
#else
  return env_put2(name, value);
//  return setenv(name, value, 1) == 0;
#endif
}
