#include <stdarg.h>
#include "../strlist.h"

int strlist_pushm_internal(strlist* sl, ...) {
  stralloc sa;
  va_list a;
  const char* s;
  stralloc_init(&sa);
  va_start(a, sl);
  while((s = va_arg(a, const char*)))
    if(stralloc_cats(&sa, s) == 0) {
      va_end(a);
      return 0;
    }
  va_end(a);
  return strlist_push_sa(sl, &sa);
}

