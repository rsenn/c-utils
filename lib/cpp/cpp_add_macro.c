#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

cpp_macro*
cpp_add_macro(char* name, bool is_objlike, cpp_token* body) {
  cpp_macro* m = alloc_zero(sizeof(cpp_macro));
  m->name = name;
  m->is_objlike = is_objlike;
  m->body = body;
  hashmap_put(&macros, name, m);
  return m;
}
