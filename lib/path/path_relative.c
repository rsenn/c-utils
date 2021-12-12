#include "../path.h"

int
path_relative(const char* path, stralloc* out) {
  return path_relative_b(path, str_len(path), out);
}
