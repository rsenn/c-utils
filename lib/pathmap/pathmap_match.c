#include "../pathmap_internal.h"
#include "../byte.h"

/* pathmap_match returns how many bytes of p[0..n) rule r consumes, or
 * (size_t)-1. An empty "from" matches every absolute path and consumes
 * nothing. A root rule does not apply to a path that already starts
 * with its root. */
size_t
pathmap_match(const pathmap_rule* r, const char* p, size_t n) {
  if(!n || *p != '/')
    return (size_t)-1;

  if(r->root && r->tlen && n >= r->tlen && byte_equal(p, r->tlen, r->to) &&
     (n == r->tlen || p[r->tlen] == '/' || r->to[r->tlen - 1] == '/'))
    return (size_t)-1;

  if(!r->flen)
    return 0;

  if(n < r->flen || !byte_equal(p, r->flen, r->from))
    return (size_t)-1;

  if(n > r->flen && p[r->flen] != '/')
    return (size_t)-1;

  return r->flen;
}
