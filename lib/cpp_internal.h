

#include "cpp.h"


void          cpp_add_macro(struct cpp*, const char* name, struct macro* m);
int           cpp_evaluate_condition(struct cpp*, struct tokenizer_s* t, int* result, char* visited[]);
int           cpp_expand_macro(struct cpp*, struct tokenizer_s* t, buffer* out, const char* name, unsigned rec_level, char* visited[]);
void          cpp_free_macros(struct cpp*);
unsigned      cpp_get_macro_info(struct cpp*, struct tokenizer_s* t, struct macro_info* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level);
struct macro* cpp_get_macro(struct cpp*, const char* name);
int           cpp_include_file(struct cpp*, struct tokenizer_s* t, buffer* out);
int           cpp_parse_file(struct cpp*, buffer* f, const char* fn, buffer* out);
int           cpp_parse_macro(struct cpp*, struct tokenizer_s* t);
int           cpp_stringify(struct cpp*, struct tokenizer_s* t, buffer* output);
int           cpp_tchain_parens_follows(struct cpp*, int rec_level);
int           cpp_undef_macro(struct cpp*, const char* name);
