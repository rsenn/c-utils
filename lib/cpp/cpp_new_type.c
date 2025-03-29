#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_type*
cpp_new_type(cpp_type_kind kind, int size, int align) {
  cpp_type* ty = alloc_zero(sizeof(cpp_type));

  ty->kind = kind;
  ty->size = size;
  ty->align = align;

  return ty;
}
