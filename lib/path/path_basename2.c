#include "../path.h"

char*
path_basename2(const char* path, size_t len) {
  size_t pos = path_basepos2(path, len);

  return &path[pos];
}
