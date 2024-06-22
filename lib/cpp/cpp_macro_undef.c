#include "../cpp_internal.h"

int
cpp_macro_undef(cpp_t* cpp, const char* name) {
  MAP_PAIR_T k;
  cpp_macro* m;

  if(!MAP_SEARCH(cpp->macros, name, str_len(name) + 1, &k))
    return 0;

  m = MAP_VALUE(k);
  /*  alloc_free(hbmap_getkey(cpp->macros, k));*/

  /*buffer_free(&m->str_contents);
  m->str_contents_buf=0;*/
  alloc_free(m->str_contents_buf);

  LIST_DESTROY(m->argnames);
  MAP_DELETE(cpp->macros, MAP_KEY(k), MAP_KEY_LEN(k));

  return 1;
}
