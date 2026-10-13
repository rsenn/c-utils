#include "../path.h"

int
path_relative_s(const char* path, stralloc* out) {
  return path_relative(path, str_len(path), out);
}
