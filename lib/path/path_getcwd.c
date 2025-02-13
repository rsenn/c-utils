#include "../path_internal.h"
#include "../windoze.h"
#include "../utf8.h"
#if WINDOWS_NATIVE
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <limits.h>

/* get current working directory into a stralloc */
void
path_getcwd(stralloc* sa) {
#if WINDOWS_NATIVE
  const size_t len = PATH_MAX;
  wchar_t w[len + 1];

  stralloc_zero(sa);
  stralloc_ready(sa, PATH_MAX * 4);

  if(_wgetcwd(w, len)) {
    sa->len = wcstou8s(sa->s, w, sa->a);
  }
#else
  char *p, sep;

  stralloc_zero(sa);

  /* reserve some space */
  stralloc_ready(sa, PATH_MAX);

  /* repeat until we have reserved enough space */
  p = getcwd(sa->s, sa->a);
  sa->len = str_len(sa->s);

  if((sep = path_getsep(sa->s)) && sep != PATHSEP_C)
    stralloc_replacec(sa, sep, PATHSEP_C);

  stralloc_nul(sa);
#endif
}

/* non-reentrant */
char*
path_getcwd_s(void) {
#if WINDOWS_NATIVE
  const size_t len = PATH_MAX;
  wchar_t w[len + 1];
  static char buf[PATH_MAX * 4 + 1];

  if(_wgetcwd(w, len)) {
    size_t n = wcstou8s(buf, w, sizeof(buf));

    if(n < sizeof(buf))
      buf[n] = '\0';

    return buf;
  }

  return NULL;
#else
  static char pathbuf[PATH_MAX];

  return getcwd(pathbuf, sizeof(pathbuf));
#endif
}
