#include "../cpp_internal.h"

struct cpp*
cpp_new(void) {
  struct cpp* ret = alloc_zero(sizeof(struct cpp));
  if(!ret)
    return ret;
  LIST_NEW(ret->includedirs);
  cpp_add_includedir(ret, ".");

  MAP_NEW(ret->macros);
  //  ret->macros = hbmap_new(strptrcmp, string_hash, 128);
  struct macro m = {.num_args = 1};
  cpp_add_macro(ret, str_dup("defined"), &m);
  m.num_args = MACRO_FLAG_OBJECTLIKE;
  cpp_add_macro(ret, str_dup("__FILE__"), &m);
  cpp_add_macro(ret, str_dup("__LINE__"), &m);
  return ret;
}
