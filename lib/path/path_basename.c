/* from dietlibc by felix leitner, adapted to libowfat */
#include "../path_internal.h"
#include "../utf8.h"

/*
       path           dirname        basename
       "/usr/lib"     "/usr"         "lib       usr/"        "/"            "usr       sr"          "."            "usr       "            "/"            "/       "            "."
   ".       ."           "."            ".."
*/
char*
path_basename(const char* path) {
  char* x = (char*)path;
  size_t n;

again:
  n = u8s_rchrs(x, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);

  if(x[n] == '\0')
    return x;

  if(x[n + 1] == 0) {
    if(n == 0)
      return x;

    (x)[n] = 0;
    goto again;
  }

  return &x[n + 1];
}
