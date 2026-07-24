#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"


cpp_macro*
cpp_macro_add(char* name, bool is_objlike, cpp_token* body) {
  cpp_macro* m;

  if((m = alloc_zero(sizeof(cpp_macro)))) {
    m->name = name;
    m->is_objlike = is_objlike;
    m->body = body;

    hashmap_put(&(cpp_ctx_get()->macros), name, m);

    if((cpp_ctx_get()->macro_ptr) == 0)
      (cpp_ctx_get()->macro_ptr) = &(cpp_ctx_get()->macro_list);

    *(cpp_ctx_get()->macro_ptr) = m;
    (cpp_ctx_get()->macro_ptr) = &m->next;

    //*(cpp_ctx_get()->macro_ptr)=0;
  }

  return m;
}

void
cpp_undefine(char* name) {
  /*cpp_macro* m = hashmap_get(&(cpp_ctx_get()->macros), name);

  if(m) {
    alloc_free(m->name);
    alloc_free(m);
  }*/

  hashmap_delete(&(cpp_ctx_get()->macros), name);
}
