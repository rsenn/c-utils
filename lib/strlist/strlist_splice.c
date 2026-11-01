#include "../strlist.h"
#include "../byte.h"
#include "../stralloc.h"

/* deletes ndelete elements from element number pos on and puts the n bytes at
 * buf there as ONE new element (n == 0 and buf == NULL: delete only). pos
 * counts elements, not bytes. 1 on success. */
int
strlist_splice(strlist* l, size_t pos, size_t ndelete, char* buf, size_t len) {
  size_t count = strlist_count(l), i;
  strlist tail;
  int64 at;

  if(pos > count)
    pos = count;
  if(ndelete > count - pos)
    ndelete = count - pos;

  /* keep a copy of what follows the deleted elements, cut the list at pos, and
   * put the new element and the copied tail back */
  strlist_init(&tail, l->sep);
  for(i = pos + ndelete; i < count; ++i) {
    size_t n;
    const char* x = strlist_at_n(l, i, &n);

    if(!strlist_pushb(&tail, x, n))
      goto fail;
  }
  at = pos < count ? strlist_pos(l, pos) : (int64)l->sa.len;
  if(pos > 0 && pos >= count)
    at = (int64)l->sa.len;
  stralloc_trunc(&l->sa, pos > 0 ? (size_t)at - (pos < count ? 1 : 0) : 0);
  if(buf && !strlist_pushb(l, buf, len))
    goto fail;
  for(i = 0; i < strlist_count(&tail); ++i) {
    size_t n;
    const char* x = strlist_at_n(&tail, i, &n);

    if(!strlist_pushb(l, x, n))
      goto fail;
  }
  stralloc_free(&tail.sa);
  return 1;
fail:
  stralloc_free(&tail.sa);
  return 0;
}
