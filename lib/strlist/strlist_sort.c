#include "../str.h"
#include "../strlist.h"

size_t
strlist_sort(strlist* sl) {
  size_t p, l = 0;
  stralloc ptrs;
  stralloc_init(&ptrs);

  for(p = 0; p < sl->sa.len; ++p) {
    size_t len;
    char* str = &sl->sa.s[p];

    if(sl->sa.s[p] == '\0')
      break;

    len = str_len(&sl->sa.s[p]);

    stralloc_catb(&ptrs, (const char *) & str, sizeof(str));
    p += len;
     ++l;
  }
  return l;
}
