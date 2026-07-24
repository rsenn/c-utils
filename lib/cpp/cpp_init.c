#include "../cpp_internal.h"
#include "../byte.h"

void
cpp_init(cpp_ctx* pp) {
  byte_zero(pp, sizeof(cpp_ctx));

  pp->macro_ptr = &pp->macro_list;
  pp->expr_scope = &pp->root_scope;
}
