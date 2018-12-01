#include "../path_internal.h"
#include "../strlist.h"
#include "../byte.h"

int
path_collapse(const char* path, stralloc* out) {
  const char* x;
  int count = 0;
  size_t n;
  char sep = path_getsep(path);
  strlist p, o;
  strlist_init(&p, sep);
  strlist_init(&o, sep);
  strlist_froms(&p, path, sep == '\\' ? '/' : '\\');

  stralloc_zero(out);

  byte_copy(&o.sa, sizeof(stralloc), out);

  strlist_foreach(&p, x, n) {
    if(count > 1) {
      if(n == 1 && *x == '.') {
        continue;
      }
      if(n == 2 && byte_equal(x, 2, "..")) {
        strlist_pop(&o);
        --count;
        continue;
      }
    }
    strlist_pushb(&o, x, n);
    ++count;
  }
  byte_copy(out, sizeof(stralloc), &o.sa);
  return count;
}
