#include "../cpp_internal.h"

int
cpp_macro_undef(cpp* pp, const char* name) {
  MAP_PAIR_T k;

  if(MAP_SEARCH(pp->macros, name, str_len(name) + 1, &k)) {
    cpp_macro* m = MAP_VALUE(k);

    /*  alloc_free(hbmap_getkey(pp->macros, k));*/
    /*buffer_free(&m->str_contents);
    m->str_contents_buf=0;*/

    if(m->str_contents_buf)
      alloc_free(m->str_contents_buf);

    LIST_DESTROY(m->argnames);
    MAP_DELETE(pp->macros, MAP_KEY(k), MAP_KEY_LEN(k));

    alloc_free(m);

    return 1;
  }

  return 0;
}
