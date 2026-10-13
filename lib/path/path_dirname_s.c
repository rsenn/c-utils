#include "../path_internal.h"
#include "../str.h"

char*
path_dirname_s(const char* path, stralloc* dir) {
  return path_dirname(path, str_len(path), dir);
}
