#include "../path_internal.h"

/* path_dirlen returns the length of the directory part of p[0..n), root-aware; 0 if there is none.
 *
 *   "/usr/lib/" -> 4 ("/usr")    "/usr" -> 1 ("/")    "a//b" -> 1 ("a")    "usr" -> 0
 */
size_t
path_dirlen(const char* p, size_t n) {
  size_t root = path_rootlen(p, n), e = n;

  while(e > root && path_issep(p[e - 1]))
    --e;

  while(e > root && !path_issep(p[e - 1]))
    --e;

  while(e > root && path_issep(p[e - 1]))
    --e;

  return e;
}
