#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_type*
cpp_type_new(cpp_type_kind kind, int size, int align) {
  cpp_type* ty;

  if((ty = alloc_zero(sizeof(cpp_type)))) {
    ty->kind = kind;
    ty->size = size;
    ty->align = align;
  }

  return ty;
}

cpp_type*
cpp_type_copy(cpp_type* ty) {
  cpp_type* ret;

  if((ret = alloc_zero(sizeof(cpp_type)))) {
    *ret = *ty;

    ret->origin = ty;
  }

  return ret;
}

void
cpp_type_free(cpp_type* ty) {
  alloc_free(ty);
}
