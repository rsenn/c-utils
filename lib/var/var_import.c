#include "../str.h"
#include "../vartab.h"
#include <assert.h>

/* add a new variable using the supplied var struct rather
 * than a malloced
 */
struct var*
var_import(const char* v, int flags, struct var* var) {
  struct search ctx;
  struct var* newvar;

  /* we should be in the root to import vars */
  assert(varstack == &vartab_root);

  /* variable name must be valid! */
  if(!var_valid(v))
    return var;

  /* search if the var already exists */
  vartab_hash(varstack, v, &ctx);

  if(!(newvar = var_search(varstack, v, &ctx))) {
    /* if not we take the supplied var struct,  */
    newvar = var;
    var = NULL;
    var_init(v, newvar, &ctx);

    /* ...and then add it to the table */
    vartab_add(varstack, newvar, &ctx);
  } else if(flags & V_INIT)
    return var;

  newvar->flags = flags;
  newvar->sa.s = (char*)v;
  newvar->sa.len = str_len(v);

  if(newvar->sa.len > newvar->len)
    newvar->offset = newvar->len + 1;

  return newvar;
}
