#include "../windoze.h"
#include "../path_internal.h"
#include "../stralloc.h"
#include "../strlist.h"

#if WINDOWS
#include <shlwapi.h>
#endif

int
path_relative(const char* path, const char* to, stralloc* rel) {
  size_t i, j, len, n;
  strlist p, t;
  stralloc cwd;
  strlist_init(&p, PATHSEP_C);
  strlist_init(&t, PATHSEP_C);
  stralloc_init(&cwd);
  stralloc_zero(rel);

  if(path[0] == '\0') path = ".";
  if(to[0] == '\0') to = ".";

#if WINDOWS
  stralloc_copys(&p.sa, path);
  path_absolute_sa(&p.sa);
  stralloc_replace(&p.sa, '/', '\\');
  stralloc_nul(&p.sa);

  stralloc_copys(&t.sa, to);
  path_absolute_sa(&t.sa);
  stralloc_replace(&t.sa, '/', '\\');
  stralloc_nul(&t.sa);

  if(!stralloc_ready(rel, PATH_MAX + 1)) return 0;

  PathRelativePathToA(
      (LPSTR)rel->s, (LPCSTR)t.sa.s, FILE_ATTRIBUTE_DIRECTORY, (LPCSTR)p.sa.s, FILE_ATTRIBUTE_DIRECTORY);

  rel->len = str_len(rel->s);

  if(rel->len >= 2 && rel->s[0] == '.' && rel->s[1] == '\\') stralloc_remove(rel, 0, 2);

  stralloc_shrink(rel);
#else
  if(!path_absolute(path) || !path_absolute(to)) path_getcwd(&cwd);

  path_realpath(path, &p.sa, 1, &cwd);
  path_realpath(to, &t.sa, 1, &cwd);

  len = strlist_count(&p);
  n = strlist_count(&t);

  if(n < len) len = n;

  for(i = 0; i < len; ++i) {

    stralloc sa1, sa2;
    sa1 = strlist_at_sa(&p, i);
    sa2 = strlist_at_sa(&t, i);

    if(stralloc_diff(&sa1, &sa2)) break;
  }

  for(j = i; j < n; ++j) {
    if(rel->len) stralloc_catc(rel, PATHSEP_C);
    stralloc_cats(rel, "..");
  }

  while(i < len) {
    char* s = strlist_at_n(&p, i, &n);
    if(rel->len) stralloc_catc(rel, PATHSEP_C);
    stralloc_catb(rel, s, n);
    ++i;
  }
#endif
  stralloc_nul(rel);
  return 1;
}