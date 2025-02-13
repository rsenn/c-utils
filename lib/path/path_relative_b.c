#include "../path.h"

int
path_relative_b(const char* path, size_t len, stralloc* out) {
  int ret;
  stralloc cwd;

  stralloc_init(&cwd);
  path_getcwd(&cwd);
  ret = path_relative_to_b(path, len, cwd.s, cwd.len, out);
  stralloc_free(&cwd);
  return ret;
}
