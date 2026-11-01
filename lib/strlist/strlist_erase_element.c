#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* removes the element that starts at byte offset pos and is len bytes long,
 * together with ONE separator, so that no empty element is left behind: the
 * one after it, or for the last element the one before it */
int
strlist_erase_element(strlist* l, size_t pos, size_t len) {
  if(pos > l->sa.len || len > l->sa.len - pos)
    return 0;
  if(pos + len < l->sa.len)
    ++len; /* the separator after the element */
  else if(pos > 0) {
    --pos; /* the last element: take the separator before it */
    ++len;
  }
  stralloc_remove(&l->sa, pos, len);
  return 1;
}
