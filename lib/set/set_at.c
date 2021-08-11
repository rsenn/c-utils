#include "../set.h"

char*
set_at(const set_t* set, int64 pos) {
  return set_at_n(set, pos, 0);
}
