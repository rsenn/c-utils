#include "../cpp_internal.h"

int
cpp_macro_undef(cpp* pp, const char* name) {
  MAP_PAIR_T k;

  if(MAP_SEARCH(pp->macros, name, str_len(name), &k)) {
    cpp_macro* macro = MAP_VALUE(k);

    /*  alloc_free(hbmap_getkey(pp->macros, k));*/
    /*buffer_free(&macro->str_contents);
    macro->str_contents_buf=0;*/

    if(macro->str_contents_buf)
      alloc_free(macro->str_contents_buf);

    LIST_DESTROY(macro->argnames);
    MAP_DELETE(pp->macros, MAP_KEY(k), MAP_KEY_LEN(k));

    //alloc_free(macro);

    return 1;
  }

  return 0;
}
