#include "../set.h"
#include "../str.h"

int
set_has_s(const set_t* set, const char* s) {
  return set_has(set, s, str_len(s));
}
