#include "../path_internal.h"
int
path_canonical(const char* path, stralloc* out) {
  return path_collapse(path, out);
}
