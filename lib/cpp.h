/**
 * @defgroup   cpp
 * @brief      CPP module.
 * @{
 */
#ifndef CPP_H
#define CPP_H

#include "buffer.h"
#include "tokenizer.h"

typedef struct cpp_s cpp_t;

struct macro_s;
struct macro_info_s;

int cpp_add_define(cpp_t*, const char* mdecl);
void cpp_add_includedir(cpp_t*, const char* includedir);
void cpp_add_macro(cpp_t*, const char* name, struct macro_s* m);
int cpp_evaluate_condition(cpp_t*, tokenizer* t, int* result, char* visited[]);
int cpp_expand_macro(cpp_t*, tokenizer* t, buffer* out, const char* name, unsigned rec_level, char* visited[]);
void cpp_free(cpp_t*);
void cpp_free_macros(cpp_t*);
struct macro_s* cpp_get_macro(cpp_t*, const char* name);
unsigned
cpp_get_macro_info(cpp_t*, tokenizer* t, struct macro_info_s* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level);
int cpp_include_file(cpp_t*, tokenizer* t, buffer* out);
cpp_t* cpp_new(void);
int cpp_parse_file(cpp_t*, buffer* f, const char* fn, buffer* out);
int cpp_parse_macro(cpp_t*, tokenizer* t);
int cpp_run(cpp_t*, buffer* in, buffer* out, const char* inname);
int cpp_stringify(cpp_t*, tokenizer* t, buffer* output);
int cpp_tchain_parens_follows(cpp_t*, int rec_level);
int cpp_undef_macro(cpp_t*, const char* name);

#endif
/** @} */
