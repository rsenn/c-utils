#include "internal.h"
#include "../str.h"

static int
lookup_exists(const char* p, void* arg) {
  (void)arg;
  return path_exists(p);
}

/* sets out to the first "dir/cmd" of the PATH-style list that exists; returns 1, or 0 with out empty.
 * ----------------------------------------------------------------------- */
int
path_lookup(stralloc* out, const char* list, const char* cmd) {
  char buf[PATH_MAX];
  size_t n = list ? path_search(buf, sizeof(buf), list, str_len(list), cmd, str_len(cmd), lookup_exists, 0) : 0;

  stralloc_zero(out);

  if(!n)
    return 0;

  stralloc_copyb(out, buf, n);
  stralloc_nul(out);
  return 1;
}
