#include "../set.h"

char*
set_at(const set_t* set, const size_t pos) {
  return set_at_n(set, pos, 0);
}
