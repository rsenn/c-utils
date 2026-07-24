#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"


cpp_macro*
cpp_macro_add(cpp_ctx* pp, char* name, bool is_objlike, cpp_token* body) {
  cpp_macro* m;

  if((m = alloc_zero(sizeof(cpp_macro)))) {
    m->name = name;
    m->is_objlike = is_objlike;
    m->body = body;

    hashmap_put(&pp->macros, name, m);

    if(pp->macro_ptr == 0)
      pp->macro_ptr = &pp->macro_list;

    *pp->macro_ptr = m;
    pp->macro_ptr = &m->next;

    //*pp->macro_ptr=0;
  }

  return m;
}

void
cpp_undefine(cpp_ctx* pp, char* name) {
  /*cpp_macro* m = hashmap_get(&pp->macros, name);

  if(m) {
    alloc_free(m->name);
    alloc_free(m);
  }*/

  hashmap_delete(&pp->macros, name);
}
