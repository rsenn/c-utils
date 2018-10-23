#include "../path.h"
#include "../path_internal.h"

int
path_absolute_sa(stralloc* sa) {
  stralloc_nul(sa);

  if(!path_isabs(sa->s)) {

    stralloc tmp = *sa;
    sa->s = 0;
    sa->a = sa->len = 0;

    path_getcwd(sa);
    stralloc_catc(sa, PATHSEP_C);
    stralloc_cat(sa, &tmp);
    stralloc_free(&tmp);
    return 1;
  }
  return 0;
}
