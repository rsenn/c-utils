#include "../vartab.h"
#include <stdlib.h>

/* get a variable
 * ----------------------------------------------------------------------- */
const char*
var_get(struct vartab* varstack, const char* v, unsigned long* offset) {
  struct var* var;
  if((var = var_search(varstack, v, NULL))) {
    if(offset) *offset = var->offset;
    return var->sa.s;
  }
  return 0;
}
