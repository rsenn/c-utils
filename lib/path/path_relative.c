#include "../path_internal.h"

int
path_relative(const char* path, const char* to, stralloc* rel) {
  size_t i, len, n;
  strlist p, t;
  stralloc cwd;
  strlist_init(&p, PATHSEP_C);
  strlist_init(&t, PATHSEP_C);
  stralloc_init(&cwd);
  if(!path_absolute(path) || !path_absolute(to))
    path_getcwd(&cwd);

  path_realpath(path, &p, 1, &cwd);
  path_realpath(to, &t, 1, &cwd);

  len = strlist_count(&p);
  n = strlist_count(&t);
  if(n < len)
    len = n;

  for(i = 0; i < len; ++i) {

    stralloc sa1, sa2;
    sa1 = strlist_at_sa(&p, i);
    sa2 = strlist_at_sa(&t, i);

    if(stralloc_diff(&sa1, &sa2))
      break;
  }
}