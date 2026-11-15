#include "../pathmap_internal.h"

/* pathmap_apply appends p[0..n) rewritten by the first rule that
 * matches. Returns 1 if one did, 0 if p was copied as it is, -1 on
 * out of memory. */
int
pathmap_apply(const pathmap* m, const char* p, size_t n, stralloc* out) {
  const pathmap_rule* r = (const pathmap_rule*)array_start(&m->rules);
  size_t i, count = (size_t)array_bytes(&m->rules) / sizeof(*r);
  size_t used;

  for(i = 0; i < count; i++) {
    if((used = pathmap_match(&r[i], p, n)) == (size_t)-1)
      continue;

    if(!stralloc_catb(out, r[i].to, r[i].tlen))
      return -1;

    if(r[i].root)
      used = 0;

    return stralloc_catb(out, p + used, n - used) ? 1 : -1;
  }

  return stralloc_catb(out, p, n) ? 0 : -1;
}
