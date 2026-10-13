#include "../path_internal.h"

char*
path_dirname_sa(stralloc* dir) {
  dir->len = path_dirlen(dir->s, dir->len);
  stralloc_nul(dir);
  return dir->s;
}
