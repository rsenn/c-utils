#include "../path.h"

char*
path_basename(const char* path) {
  size_t pos = path_basepos2(path, str_len(path));

  return &path[pos];
}
