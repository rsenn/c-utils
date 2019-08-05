#include "../path_internal.h"

char*
path_skip_separator(const char* p) {
  while(*p && path_issep(*p)) ++p;
  return (char*)p;
}

