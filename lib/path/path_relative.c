#include "../path.h"

int
path_relative(const char* path, stralloc* out) {
  int ret;
  stralloc cwd;
  path_getcwd(&cwd);
  ret=path_relative_to_b(path, str_len(path), cwd.s, cwd.len, out);
  stralloc_free(&cwd);
  return ret;
}
