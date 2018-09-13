#include "../stralloc.h"
#include "../buffer.h"
#include "../tree.h"
#include "../vartab.h"
#include "../expand.h"

/* evaluates backquoted command list, while writing stdout to a stralloc 
 * ----------------------------------------------------------------------- */
union node*
expand_command(struct nargcmd *cmd, union node **nptr, struct vartab* varstack, int flags) {
  union node *n = *nptr;
  stralloc sa;
  stralloc_init(&sa);

  /* make the output buffer write to the stralloc */
  buffer_tosa(buffer_1, &sa); 

  /* evaluate the command tree in a subshell */
  /*sh_push(&sh);
  sh_subshell(cmd->list, E_EXIT);
  sh_pop(&sh);*/

  /* split trailing newlines */
  while(sa.len && sa.s[sa.len - 1] == '\n')
    sa.len--;

  /* expand the output of the command 
   
     FIXME: we could do this much nicer by doing an
            expand_write() which is set as buffer op 
            on the output fd.
   
            so we won't have to alloc all the stuff twice!
   */
  n = expand_cat(sa.s, sa.len, nptr, varstack, flags);
  stralloc_free(&sa);

  return n;
}

