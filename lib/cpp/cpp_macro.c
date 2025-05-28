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

    hashmap_put(&cpp_macros, name, m);

    if(cpp_macro_ptr == 0)
      cpp_macro_ptr = &cpp_macro_list;

    *cpp_macro_ptr = m;
    cpp_macro_ptr = &m->next;

    //*cpp_macro_ptr=0;
  }

  return m;
}

void
cpp_undefine(char* name) {
  /*cpp_macro* m = hashmap_get(&cpp_macros, name);

  if(m) {
    alloc_free(m->name);
    alloc_free(m);
  }*/

  hashmap_delete(&cpp_macros, name);
}
