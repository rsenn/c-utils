#include "../env.h"
#include "../str.h"

char*
env_pick(void) {
#if !WINDOWS_NATIVE
  return environ[0];
#endif
}
