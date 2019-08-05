#include "../vartab.h"
#include "../stralloc.h"
#include "../tree.h"
#include "../expand.h"

/* expand all arguments of an argument list
 * returns count of argument nodes
 */
int
expand_args(union node* args, union node** nptr, struct vartab* varstack, char *argv[], int exitcode, int flags) {
  union node* arg;
  union node* n;
  int ret = 0;

  *nptr = NULL;

  for(arg = args; arg; arg = arg->list.next) {
    if((n = expand_arg(&arg->narg, nptr, varstack, argv, exitcode, flags))) {
      nptr = &n;
      ret++;
    }

    if(n == NULL) continue;

    if(n->narg.flag & X_GLOB) {
      if((n = expand_glob(nptr, varstack, n->narg.flag & ~X_GLOB))) {
        nptr = &n;
        ret++;
      }
    } else {
      expand_unescape(&n->narg.stra);
      n->narg.flag &= ~X_GLOB;
    }

    if(arg->list.next) {
      n->list.next = tree_newnode(N_ARG);
      n = n->list.next;
      stralloc_init(&n->narg.stra);
      ret++;
    }
  }

  return ret;
}

