#include "../cpp_internal.h"

cpp_t*
cpp_new(void) {
  cpp_t* ret;
  cpp_macro m = {/*.num_args =*/1};

  if(!(ret = alloc_zero(sizeof(struct cpp_s))))
    return ret;

  LIST_NEW(ret->includedirs);
  cpp_add_includedir(ret, ".");

  MAP_NEW(ret->macros);

  cpp_macro_add(ret, "defined", &m);

  m.num_args = MACRO_FLAG_OBJECTLIKE;
  cpp_macro_add(ret, "__FILE__", &m);
  cpp_macro_add(ret, "__LINE__", &m);

  return ret;
}
