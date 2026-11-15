#include "../pathmap.h"
#include "../byte.h"

/* pathmap_addb adds the rule spec[0..n) = "from=to" to m, trailing
 * slashes of from and to are dropped. Returns 0 for a rule without "="
 * unless it is a root rule. */
int
pathmap_addb(pathmap* m, const char* spec, size_t n, int root) {
  pathmap_rule r;
  size_t i = byte_chr(spec, n, '=');

  if(i == n && !root)
    return 0;

  r.root = !!root;
  r.from = spec;
  r.flen = i == n ? 0 : i;
  r.to = i == n ? spec : spec + i + 1;
  r.tlen = i == n ? n : n - i - 1;

  while(r.flen > 1 && r.from[r.flen - 1] == '/')
    r.flen--;

  while(r.tlen > 1 && r.to[r.tlen - 1] == '/')
    r.tlen--;

  array_catb(&m->rules, &r, sizeof(r));
  return !array_failed(&m->rules);
}
