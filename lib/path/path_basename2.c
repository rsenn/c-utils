/* from dietlibc by felix leitner, adapted to libowfat */
#include "../path_internal.h"
#include "../utf8.h"

/*
       path           dirname        basename
       "/usr/lib"     "/usr"         "lib       usr/"        "/"            "usr       sr"          "."            "usr       "            "/"            "/       "            "."
   ".       ."           "."            ".."
*/
char*
path_basename2(const char* path, size_t len) {
  char* x = (char*)path;
  size_t n;

again:
  n = u8b_rchrs(x, len, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);

  if(n == len)
    return x;

  if(n + 1 == len) {
    if(n == 0)
      return x;

    (x)[n] = 0;
    goto again;
  }

  return &x[n + 1];
}
