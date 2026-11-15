#ifndef PATHMAP_H
#define PATHMAP_H

#include "array.h"
#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pathmap rewrites absolute paths by an ordered list of rules. A rule
 * matches a directory prefix at a component boundary, so "/mingw64"
 * matches "/mingw64" and "/mingw64/lib" but not "/mingw64x". The first
 * matching rule wins, a path no rule matches stays as it is. The rule
 * strings are not copied: the caller keeps them alive. */
typedef struct pathmap_rule {
  const char* from;
  size_t flen;
  const char* to;
  size_t tlen;
  unsigned root : 1;
} pathmap_rule;

typedef struct pathmap {
  array rules;
} pathmap;

/* pathmap_addb adds the rule "from=to" from spec[0..n). With root zero
 * the matched prefix is replaced by to. With root set to is a root
 * directory put in front of the whole path, as PKG_CONFIG_SYSROOT_DIR
 * does, and a path that already starts with to is left alone; a spec
 * without "=" is then a root for every absolute path. Returns 1, or 0
 * when spec is malformed or out of memory. */
int pathmap_addb(pathmap* m, const char* spec, size_t n, int root);

/* pathmap_addlist adds every ':' separated rule of list, see
 * pathmap_addb. Returns the number of rules added. */
size_t pathmap_addlist(pathmap* m, const char* list, int root);

/* pathmap_apply appends the rewritten p[0..n) to out. Returns 1 when a
 * rule matched, 0 when p was copied unchanged, -1 when out ran out of
 * memory. */
int pathmap_apply(const pathmap* m, const char* p, size_t n, stralloc* out);

/* pathmap_flags appends in[0..n) to out with the path of every -I and -L
 * word rewritten. Returns the number of words changed, -1 when out ran
 * out of memory. */
int pathmap_flags(const pathmap* m, const char* in, size_t n, stralloc* out);

#ifdef __cplusplus
}
#endif

#endif /* PATHMAP_H */
