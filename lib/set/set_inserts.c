#include "../set.h"
#include "../str.h"

int
set_inserts(set_t* set, const char* str) {
  return set_insert(set, str, str_len(str));
}
