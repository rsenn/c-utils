#include "../cpp_internal.h"

/* used by cpp_macro.c */
hashmap cpp_macros = HASHMAP_INIT();
cpp_macro* cpp_macro_list = 0;
cpp_macro** cpp_macro_ptr = &cpp_macro_list;

/* used by cpp_preprocess2.c */
hashmap cpp_pragma_once = HASHMAP_INIT();
hashmap include_guards = HASHMAP_INIT(), include_list = HASHMAP_INIT();
cpp_cond_incl* cond_incl = 0;
strarray include_array = {};

/* used by cpp_search_include.c */
strarray cpp_include_paths = STRARRAY_INIT();
int cpp_include_next_idx = 0;

/* used by cpp_tokenize.c */
int cpp_at_bol = 0, cpp_has_space = 0;
cpp_file* cpp_current_file = 0;

/* used by cpp_const_expr.c */
cpp_obj* locals = 0;

/* used by cpp_init_macros.c */
char* cpp_base_file = 0;

/* used by cpp_tokenize_file.c */
cpp_file** cpp_input_files = 0;
int cpp_file_no = 0;
