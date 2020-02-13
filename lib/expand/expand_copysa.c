#include "../alloc.h"
#include "../vartab.h"
#include "../tree.h"
#include "../byte.h"
#include "../expand.h"

/* expand one N_ARG node to a stralloc (stralloc is overwritten!!!)
 */
void
expand_copysa(union node* node, stralloc* sa, struct vartab* varstack, char* argv[], int exitcode, int flags) {
  union node tmpnode;
  union node* n = &tmpnode;

  stralloc_init(&tmpnode.narg.stra);
  expand_arg(&node->narg, &n, varstack, argv, exitcode, flags | X_NOSPLIT);
  byte_copy(sa, sizeof(stralloc), &tmpnode.narg.stra);
  stralloc_nul(sa);
}
