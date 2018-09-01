#include "../env.h"
#include "../str.h"

char*
env_pick(void) {
  return environ[0];
}
