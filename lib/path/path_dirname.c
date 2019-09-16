#include "../path_internal.h"
#include "../str.h"

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

char*
path_dirname(const char* path, stralloc* dir) {
  size_t i;
  stralloc_zero(dir);
  i = str_rchrs(path, "/\\", 2);
  if(path == NULL || path[i] == '\0') {
    stralloc_copys(dir, dot);
  } else {
    /* remove trailing slashes */
    while(i > 0 && path_issep(path[i - 1])) --i;
    stralloc_copyb(dir, path, i);
  }
  stralloc_nul(dir);
  return dir->s;
}
