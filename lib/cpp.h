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

int cpp_add_define(cpp*, const char*);
void cpp_add_includedir(cpp*, const char*);
int cpp_evaluate_condition(cpp*, tokenizer*, int*, char* visited[]);
void cpp_free(cpp*);
int cpp_include_file(cpp*, tokenizer*, buffer*);
int cpp_macro_add(cpp*, const char*, cpp_macro*);
int cpp_macro_expand(cpp*, tokenizer*, buffer*, const char*, unsigned, char* visited[]);
cpp_macro* cpp_macro_get(cpp*, const char*);
unsigned cpp_macro_get_info(cpp*, tokenizer*, cpp_macro_info*, size_t*, unsigned, unsigned, const char*, char* visited[], unsigned);
int cpp_macro_parse(cpp*, tokenizer*);
int cpp_macro_undef(cpp*, const char*);
cpp* cpp_new(void);
int cpp_parse_file(cpp*, buffer*, const char*, buffer*);
int cpp_run(cpp*, buffer*, buffer*, const char*);
int cpp_stringify(cpp*, tokenizer*, buffer*);
int cpp_tchain_parens_follows(cpp*, int);

#endif
/** @} */
