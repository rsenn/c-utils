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
        NULL           "."            "."
        ""             "."            "."
*/

static char* dot = ".";
#define SLASH '/'
#define EOL (char)0
char*
path_dirname(char* path) {
  size_t i;
  if(path == NULL) return dot;
  for(;;) {
    i = str_rchr(path, SLASH);
    if(path[i] == '\0') return dot; /* no slashes */

    if(path[i + 1] == EOL && i) {
      /* remove trailing slashes */
      while(path[i] == SLASH && i) path[i--] = EOL;
      continue;
    }

    if(i)
      while(path[i] == SLASH) path[i--] = EOL; /* slashes in the middle */
    else
      path[1] = EOL; /* slash is first symbol */

    return path;
  }
}
