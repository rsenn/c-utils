#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_type*
cpp_type_copy(cpp_type* ty) {
  cpp_type* ret;

  if((ret = alloc_zero(sizeof(cpp_type)))) {
    *ret = *ty;

    ret->origin = ty;
  }

  return ret;
}
