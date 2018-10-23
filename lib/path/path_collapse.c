#include "../path_internal.h"
#include "../strlist.h"

int
path_collapse(const char* path, stralloc* out) {
  const char* x;
  size_t i = 0, n;
  strlist p, o;
  strlist_init(&p, PATHSEP_C);
  strlist_init(&o, PATHSEP_C);
  strlist_froms(&p, path, PATHSEP_C == '\\' ? '/' : '\\');

  stralloc_zero(out);
  byte_copy(&o.sa, sizeof(stralloc), out);

  strlist_foreach(&p, x, n) {

    if(i > 1) {

      if(n == 2 && byte_equal(x, 2, "..")) {
        strlist_pop(&o);
        --i;
        continue;
      }
    }

    strlist_pushb(&o, x, n);
    ++i;
  }

  byte_copy(out, sizeof(stralloc), &o.sa);
  return strlist_count(&o);
}