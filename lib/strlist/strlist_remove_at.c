#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes the element number n; 1 on success, 0 if there is none */
int
strlist_remove_at(strlist* l, size_t n) {
  size_t len;
  const char* x = strlist_at_n(l, n, &len);

  if(!x)
    return 0;
  return strlist_erase_element(l, (size_t)(x - l->sa.s), len);
}
