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

  stralloc_zero(sa);
  /* reserve some space */
  stralloc_ready(sa, PATH_MAX);
  /* repeat until we have reserved enough space */
  getcwd(sa->s, sa->a);

  /* now truncate to effective length */
  stralloc_trunc(sa, str_len(sa->s));
}
