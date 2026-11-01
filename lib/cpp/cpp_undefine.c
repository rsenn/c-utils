#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"
#include "../str.h"

void
cpp_undefine(cpp_ctx* pp, char* name) {
  /*cpp_macro* m = hashmap_get(&pp->macros, name);

  if(m) {
    alloc_free(m->name);
    alloc_free(m);
  }*/

  hashmap_delete(&pp->macros, name);
}
