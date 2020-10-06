#include "../set.h"
#include "../str.h"

int
set_adds(set_t* set, const char* str) {
  return set_add(set, str, str_len(str));
}
