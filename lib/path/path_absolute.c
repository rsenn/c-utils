#include "../path_internal.h"

int
path_absolute(const char* path, stralloc* sa) {
  int ret = 0;
  stralloc_zero(sa);

  if(!path_isabs(path)) {
    path_getcwd(sa);

    if(!str_equal(path, ".")) {
      stralloc_catc(sa, PATHSEP_C);
      stralloc_cats(sa, path);
    }

    return 1;
  }

  stralloc_copys(sa, path);
  return 0;
}

char*
path_absolute_s(const char* path) {
  stralloc sa;

  stralloc_init(&sa);
  path_absolute(path, &sa);
  stralloc_nul(&sa);

  return sa.s;
}
