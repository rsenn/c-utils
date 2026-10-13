#include "../path_internal.h"
#include "../str.h"
#include "../windoze.h"

/* "/x" is absolute; on Windows so is "\x" and "C:\x" ("C:" and "C:x" stay relative) */
int
path_rootlen(const char* x, size_t n) {
  if(n > 0 && path_issep(x[0]))
    return 1;

  if(n >= 2 && path_isdrive(x))
    return n >= 3 && path_issep(x[2]) ? 3 : 2;

  return 0;
}
