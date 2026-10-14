#include "../path_internal.h"

/* path_basename returns the last component of p[0..n) as a view; *len (if not NULL) gets its length.
 *
 *   "/usr/lib" -> "lib"    "usr/" -> "usr"    "/" -> "/"    "" -> ""
 */
const char*
path_basename(const char* p, size_t n, size_t* len) {
  size_t root = path_rootlen(p, n), end = n, start;

  while(end > root && path_issep(p[end - 1]))
    --end;

  start = end;

  while(start > root && !path_issep(p[start - 1]))
    --start;

  if(start == end && root) {
    start = 0;
    end = root;
  }

  if(len)
    *len = end - start;

  return p + start;
}
