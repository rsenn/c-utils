#include "../path.h"

int
path_relative_to(const char* path, const char* relative_to, stralloc* out) {
  return path_relative_to_b(path, str_len(path), relative_to, str_len(relative_to), out);
}
