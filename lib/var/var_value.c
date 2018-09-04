#include "../var.h"

/* get a variable value
 * ----------------------------------------------------------------------- */
const char*
var_value(struct vartab* varstack, const char* v, unsigned long* plen) {
  return var_vdefault(varstack, v, "", plen);
}
