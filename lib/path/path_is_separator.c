#include "../path_internal.h"

int
path_is_separator(const char* c) {
  size_t len = u8_len(c, 1);

  return (len == 1 && path_issep(*c));
}
