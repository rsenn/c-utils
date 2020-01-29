/* from dietlibc by felix leitner, adapted to libowfat */
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
*/
char*
path_basename(const char* path) {
  size_t n;
again:
  n = str_rchrs(path, "/\\", 2);
  if(path[n] == '\0')
    return path;
  if(path[n + 1] == 0) {
    if(n == 0)
      return path;
    else {
      ((char*)path)[n] = 0;
      goto again;
    }
  }
  return (char*)&path[n + 1];
}
