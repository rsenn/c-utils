#include "../windoze.h"
#include "../path_internal.h"

#if WINDOWS_NATIVE
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <limits.h>

/* get current working directory into a stralloc
 * ----------------------------------------------------------------------- */
void
path_getcwd(stralloc* sa) {
  /* do not allocate PATH_MAX from the beginning,
     most paths will be smaller */
  size_t n = PATH_MAX / 16;

  do {
    /* reserve some space */
    stralloc_ready(sa, n);
    n += PATH_MAX / 8;
    /* repeat until we have reserved enough space */
  } while(getcwd(sa->s, sa->a) == NULL);

  /* now truncate to effective length */
  stralloc_trunc(sa, str_len(sa->s));
}
