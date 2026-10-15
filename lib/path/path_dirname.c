#include "internal.h"
#include "../str.h"
#include "../byte.h"

/*
        path           dirname        basename
        "/usr/lib"     "/usr"         "lib        usr/"        "/"            "usr        sr"          "." "usr        "
   "/"            "/        " "." ". ."
   "."            ".." NULL           "."            ".                     "."            "."
*/
static const char* const dot = ".";

char*
path_dirname(const char* path, size_t size, stralloc* dir) {
  size_t i = path ? path_dirlen(path, size) : 0;

  stralloc_zero(dir);

  if(i == 0)
    stralloc_copys(dir, dot);
  else
    stralloc_copyb(dir, path, i);

  stralloc_nul(dir);
  return dir->s;
}
