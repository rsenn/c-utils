#include "../path_internal.h"
#include "../str.h"
#include "../byte.h"

/*
        path           dirname        basename
        "/usr/lib"     "/usr"         "lib"
        "/usr/"        "/"            "usr"
        "usr"          "."            "usr"
        "/"            "/"            "/"
        "."            "."            "."
        ".."           "."            ".."
        NULL           "."            "."
        ""             "."            "."
*/
static const char* const dot = ".";

size_t
path_dirlen(const char* path) {
  return str_rchrs(path, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);
}

char*
path_dirname(const char* path, stralloc* dir) {
  size_t i;
  stralloc_zero(dir);
  i = path_dirlen(path);
  if(path == NULL || path[i] == '\0') {
    stralloc_copys(dir, dot);
  } else {
    /* remove trailing slashes */
    while(i > 0 && path_issep(path[i - 1]))
      --i;
    stralloc_copyb(dir, path, i);
  }
  stralloc_nul(dir);
  return dir->s;
}
