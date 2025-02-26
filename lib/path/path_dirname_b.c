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

char*
path_dirname_b(const char* path, size_t size, stralloc* dir) {
  size_t i;

  stralloc_zero(dir);
  i = path_dirlen_b(path, size);

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
