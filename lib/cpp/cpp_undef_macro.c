#include "../cpp_internal.h"

int
cpp_undef_macro(cpp_t* cpp, const char* name) {
  MAP_ITER_T k;
  if(!MAP_SEARCH(cpp->macros, name, str_len(name) + 1, &k))
    return 0;
  struct macro_s* m = MAP_DATA(cpp->macros, k);
  /*  alloc_free(hbmap_getkey(cpp->macros, k));*/
  if(m->str_contents)
    buffer_close(m->str_contents);
  alloc_free(m->str_contents_buf);
  LIST_DESTROY(m->argnames);
  // LIST_DESTROY(m->argnames);
  MAP_DELETE(cpp->macros, MAP_KEY(cpp->macros, k), MAP_KEY_LEN(cpp->macros, k));
  return 1;
}
