#include "../path_internal.h"
#include "../strlist.h"

int
path_collapse(const char* path, stralloc* out) {
  const char* x;
  size_t i = 0, n;
  char sep = path_getsep(path);
  strlist p, o;
  strlist_init(&p, sep);
  strlist_init(&o, sep);
  strlist_froms(&p, path, sep == '\\' ? '/' : '\\');

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
  return i;
}