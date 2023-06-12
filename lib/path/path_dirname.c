#include "../path_internal.h"
#include "../str.h"

char*
path_dirname(const char* path, stralloc* dir) {
  return path_dirname_b(path, str_len(path), dir);
}
