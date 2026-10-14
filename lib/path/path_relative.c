#include "../path.h"

int
path_relative(const char* path, size_t len, stralloc* out) {
  int ret;
  stralloc cwd;

  stralloc_init(&cwd);
  path_getcwd(&cwd);
  ret = path_rel_sa(out, path, len, cwd.s, cwd.len);
  stralloc_free(&cwd);
  return ret;
}
