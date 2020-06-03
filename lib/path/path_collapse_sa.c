#include "../path_internal.h"
#include "../byte.h"

size_t
path_collapse_sa(stralloc* path) {
  size_t ret = path_collapse(path->s, path->len);
  stralloc_nul(path);
  return ret;
}
