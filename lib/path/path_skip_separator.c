#include "../path_internal.h"

char*
path_skip_separator(const char* c) {
  while(*c && path_issep(c))
  ++c;
  return c;
}
