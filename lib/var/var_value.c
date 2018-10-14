#include "../var.h"

/* get a variable value
 * ----------------------------------------------------------------------- */
const char*
var_value(struct vartab* varstack, const char* v, size_t* plen) {
  return var_vdefault(varstack, v, "", plen);
}
