#include "../vartab.h"
#include "../tree.h"
#include "../expand.h"
#include <stdlib.h>

/* expand an assignment list
 * ----------------------------------------------------------------------- */
int
expand_vars(union node* vars, union node** nptr, struct vartab* varstack, char *argv[], int exitcode) {
  union node* var;
  union node* n;
  int ret = 0;

  *nptr = NULL;

  for(var = vars; var; var = var->list.next) {
    if((n = expand_arg(&var->narg, nptr, varstack, argv, exitcode, X_NOSPLIT))) {
      nptr = &n;
      ret++;
    }

    expand_unescape(&n->narg.stra);

    if(n) nptr = &n->list.next;
  }

  return ret;
}
