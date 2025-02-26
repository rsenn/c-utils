#include "../strlist.h"
#include <stdarg.h>

int
strlist_pushm_internal(strlist* sl, ...) {
  va_list a;
  const char* s;
  va_start(a, sl);

  while((s = va_arg(a, const char*))) {
    if(strlist_push(sl, s) == 0)
      return 0;
  }
  va_end(a);
  return 1;
}
