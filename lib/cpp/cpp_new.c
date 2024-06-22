#include "../cpp_internal.h"

cpp*
cpp_new(void) {
  cpp* pp;
  cpp_macro macro = {0};

  if(!(pp = alloc_zero(sizeof(struct cpp_s))))
    return pp;

  LIST_NEW(pp->includedirs);
  cpp_add_includedir(pp, ".");

  MAP_NEW(pp->macros);

  macro.num_args = 1;
  cpp_macro_add(pp, "defined", &macro);

  macro.num_args = MACRO_FLAG_OBJECTLIKE;
  cpp_macro_add(pp, "__FILE__", &macro);
  cpp_macro_add(pp, "__LINE__", &macro);

  return pp;
}
