#include "../windoze.h"
#if WINDOWS
#include <windows.h>
#else
#include <stdlib.h>
#endif
#include "../stralloc.h"
#include "../env.h"
#include "../byte.h"
#include "../str.h"

/* Replaces references to environment variables in sa by their values:
 * %NAME% on Windows (ExpandEnvironmentStrings), $NAME and ${NAME} elsewhere.
 * An unset variable expands to nothing on Windows and stays as written
 * elsewhere. Returns 1 on success, 0 when out of memory. */
int
stralloc_expand(stralloc* sa) {
#if WINDOWS
  size_t n;
  char* s;

  stralloc_nul(sa);
  s = sa->s;
  n = sa->len * 2 + 4;
  sa->s = 0;
  sa->len = sa->a = 0;
  do {
    /* reserve some space */
    if(!stralloc_ready(sa, n)) {
      free(s);
      return 0;
    }
    /* repeat until we have reserved enough space */
  } while((n = ExpandEnvironmentStrings(s, sa->s, (DWORD)sa->a)) > sa->a);
  /* now truncate to effective length */
  if(n > 0)
    stralloc_trunc(sa, n - 1);
  free(s);
  return 1;
#else
  stralloc out;
  size_t i = 0;

  stralloc_init(&out);
  while(i < sa->len) {
    char c = sa->s[i];

    if(c == '$' && i + 1 < sa->len) {
      size_t j = i + 1, start, end;
      int brace = sa->s[j] == '{';

      if(brace)
        ++j;
      start = j;
      while(j < sa->len && (sa->s[j] == '_' || (sa->s[j] >= '0' && sa->s[j] <= '9') || (sa->s[j] >= 'A' && sa->s[j] <= 'Z') ||
                            (sa->s[j] >= 'a' && sa->s[j] <= 'z')))
        ++j;
      end = j;
      if(end > start && (!brace || (j < sa->len && sa->s[j] == '}'))) {
        char name[256];
        const char* v;

        if(end - start < sizeof name) {
          byte_copy(name, end - start, sa->s + start);
          name[end - start] = 0;
          if((v = env_get(name))) {
            if(!stralloc_cats(&out, v)) {
              stralloc_free(&out);
              return 0;
            }
            i = end + (brace ? 1 : 0);
            continue;
          }
        }
      }
    }
    if(!stralloc_catc(&out, (unsigned char)c)) {
      stralloc_free(&out);
      return 0;
    }
    ++i;
  }
  stralloc_free(sa);
  *sa = out;
  return 1;
#endif
}
