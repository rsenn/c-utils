#include "../path_internal.h"
#include "../utf8.h"

/*
       path           dirname        basename
       "/usr/lib"     "/usr"         "lib       usr/"        "/"            "usr       sr"          "."            "usr       "            "/"            "/       "            "."
   ".       ."           "."            ".."
*/
size_t
path_basepos2(const char* path, size_t len) {
  char* x = (char*)path;
  size_t n;

again:
  n = u8b_rchrs(x, len, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);

  if(n == len)
    return n;

  if(n + 1 == len) {
  len=n;
     goto again;
  }

  return n;
}
