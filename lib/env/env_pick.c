#include "../env.h"
#include "../str.h"

char*
env_pick(void) {
#if !((defined(_WIN32) || defined(_WIN64)) && !(defined(__MSYS__) || defined(__CYGWIN__)))
  return environ[0];
#endif
}
