/**
 * @defgroup   cpp
 * @brief      CPP module.
 * @{
 */
#ifndef CPP_H
#define CPP_H

#include "buffer.h"
#include "tokenizer.h"
#include "pplist.h"

typedef struct {
  unsigned num_args;
  buffer str_contents;
  char* str_contents_buf;
  LIST_T argnames;
} cpp_macro;

typedef struct {
  const char* name;
  unsigned nest;
  unsigned first;
  unsigned last;
} cpp_macro_info;

typedef struct {
  buffer* f;
  char* buf;
  size_t len;
  tokenizer t;
} cpp_file;

typedef struct cpp_s cpp_t;

int cpp_add_define(cpp_t*, const char* mdecl);
void cpp_add_includedir(cpp_t*, const char* includedir);
int cpp_evaluate_condition(cpp_t*, tokenizer* t, int* result, char* visited[]);
void cpp_free(cpp_t*);
int cpp_include_file(cpp_t*, tokenizer* t, buffer* out);
void cpp_macro_add(cpp_t*, const char* name, cpp_macro* m);
int cpp_macro_expand(cpp_t*, tokenizer* t, buffer* out, const char* name, unsigned rec_level, char* visited[]);
cpp_macro* cpp_macro_get(cpp_t*, const char* name);
unsigned cpp_macro_get_info(cpp_t*, tokenizer* t, cpp_macro_info* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], unsigned rec_level);
int cpp_macro_parse(cpp_t*, tokenizer* t);
int cpp_macro_undef(cpp_t*, const char* name);
cpp_t* cpp_new(void);
int cpp_parse_file(cpp_t*, buffer* f, const char* fn, buffer* out);
int cpp_run(cpp_t*, buffer* in, buffer* out, const char* inname);
int cpp_stringify(cpp_t*, tokenizer* t, buffer* output);
int cpp_tchain_parens_follows(cpp_t*, int rec_level);

#endif
/** @} */
