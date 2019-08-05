#include "../path_internal.h"
#include "../stralloc.h"
#include "../strlist.h"
#include "../windoze.h"
#define MAX_NUM(a, b) ((a) > (b) ? (a) : (b))
#if WINDOWS_NATIVE
#include <windows.h>
#endif
int
path_relative(const char* path, const char* relative_to, stralloc* out) {
#if 0 // WINDOWS_NATIVE
  size_t i, j, len, n;
  strlist p, t;
  stralloc cwd;
  strlist_init(&p, PATHSEP_C);
  strlist_init(&t, PATHSEP_C);
  stralloc_init(&cwd);
  stralloc_zero(rel);
  if(path[0] == '\0') path = ".";
  if(relative_to[0] == '\0') relative_to = ".";
  stralloc_copys(&p.sa, path);
  path_absolute_sa(&p.sa);
  stralloc_replace(&p.sa, '/', '\\');
  stralloc_nul(&p.sa);
  stralloc_copys(&t.sa, relative_to);
  path_absolute_sa(&t.sa);
  stralloc_replace(&t.sa, '/', '\\');
  stralloc_nul(&t.sa);
  if(!stralloc_ready(out, PATH_MAX + 1)) return 0;
  PathRelativePathToA(
      (LPSTR)out->s, (LPCSTR)t.sa.s, FILE_ATTRIBUTE_DIRECTORY, (LPCSTR)p.sa.s, FILE_ATTRIBUTE_DIRECTORY);
  out->len = str_len(out->s);
  if(out->len >= 2 && out->s[0] == '.' && out->s[1] == '\\') stralloc_remove(out, 0, 2);
  stralloc_shrink(out);
#else
  strlist rel;
  strlist p, r;
  strlist_init(&rel, PATHSEP_C);
  strlist_init(&p, PATHSEP_C);
  strlist_init(&r, PATHSEP_C);

  stralloc_zero(out);

  path_canonical(relative_to[0] ? relative_to : ".", &r.sa);
  if(stralloc_starts(&r.sa, ".."))
    path_absolute_sa(&r.sa);

  path_canonical(path[0] ? path : ".", &p.sa);
  if(stralloc_starts(&p.sa, ".."))
    path_absolute_sa(&p.sa);

  {
    size_t n1 = strlist_count(&p);
    size_t n2 = strlist_count(&r);
    size_t i, n = MAX_NUM(n1, n2);

    for(i = 0; i < n; ++i) {
      size_t l1, l2;
      char* s1 = strlist_at_n(&p, i, &l1);
      char* s2 = strlist_at_n(&r, i, &l2);
#ifdef DEBUG_OUTPUT
      buffer_puts(buffer_2, "REL ");
      buffer_put(buffer_2, s1, l1);
      buffer_puts(buffer_2, " ");
      buffer_put(buffer_2, s2, l2);
      buffer_putnlflush(buffer_2);
#endif
      if(l1 != l2)
        break;
      if(byte_diff(s1, l1, s2))
        break;
    }
    strlist_init(&rel, PATHSEP_C);
    while(n2-- > i) {
      strlist_push(&rel, "..");
    }
    while(i < n1) {
      char* s = strlist_at_n(&p, i, &n);
      strlist_pushb(&rel, s, n);
      ++i;
    }
  }
  //  strlist_join(&rel, out, PATHSEP_C);

  if(rel.sa.len == 0) {
    stralloc_copys(out, ".");
  } else {
    stralloc_move(out, &rel.sa);
  }

  strlist_free(&p);
  strlist_free(&r);
  strlist_free(&rel);
#endif
  return 1;
}

