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
  LIST_T argnames;
  union {
    char* str_contents_buf;
    buffer str_contents;
  };
} cpp_macro;

typedef struct {
  const char* name;
  unsigned nest, first, last;
} cpp_macro_info;

typedef struct {
  buffer* f;
  char* buf;
  size_t len;
  tokenizer t;
} cpp_file;

typedef struct cpp_s cpp;

int cpp_add_define(cpp*, const char* mdecl);
void cpp_add_includedir(cpp*, const char* includedir);
void cpp_dump(cpp*, buffer*);
void cpp_error_or_warning(const char*, const char* type, tokenizer* t, token* curr);
int cpp_evaluate_condition(cpp*, tokenizer* t, int* result, char* visited[]);
void cpp_free(cpp*);
int cpp_include_file(cpp*, tokenizer* t, buffer* out);
int cpp_macro_add(cpp*, const char* name, cpp_macro* macro);
size_t cpp_macro_arglist_pos(cpp_macro*, const char* iden);
int cpp_macro_expand(cpp*, tokenizer* t, buffer* out, const char* name, int rec_level, char* visited[]);
cpp_macro* cpp_macro_get(cpp*, const char* name);
unsigned cpp_macro_get_info(cpp*, tokenizer* t, cpp_macro_info* mi_list, size_t* mi_cnt, unsigned nest, unsigned tpos, const char* name, char* visited[], int rec_level);
int cpp_macro_parse(cpp*, tokenizer* t);
int cpp_macro_undef(cpp*, const char* name);
cpp* cpp_new(void);
int cpp_parse_binary(tokenizer*, int left, token* tok, int* err);
int cpp_parse_error(tokenizer*, int is_error);
int cpp_parse_expect(tokenizer*, enum tokentype tt, const char* const values[], token* token);
int cpp_parse_expr(tokenizer*, int rbp, int* err);
int cpp_parse_file(cpp*, buffer* f, const char* fn, buffer* out);
int cpp_parse_peek(tokenizer*, token* tok);
int cpp_parse_skip(tokenizer*, token* tok);
int cpp_parse_unary(tokenizer*, token* tok, int* err);
int cpp_parse_whitespace(tokenizer*, token* token, int* count);
int cpp_stringify(cpp*, tokenizer* t, buffer* out);
int cpp_tchain_parens_follows(cpp*, int rec_level);
void cpp_location( tokenizer* t,buffer*out);

#endif
/** @} */
