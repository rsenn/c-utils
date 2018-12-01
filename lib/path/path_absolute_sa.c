#include "../path_internal.h"

int
path_absolute_sa(stralloc* sa) {
  int ret = 0;
  stralloc_nul(sa);
  if(!path_isabs(sa->s)) {
    stralloc tmp;
    stralloc_init(&tmp);
    stralloc_copy(&tmp, sa);
    stralloc_zero(sa);
    path_getcwd(sa);
    stralloc_catc(sa, PATHSEP_C);
    stralloc_cat(sa, &tmp);
    stralloc_free(&tmp);
    ret = 1;
  }
  if(sa->len && path_issep(sa->s[sa->len - 1]))
    --sa->len;
  stralloc_nul(sa);
  return ret;
}
