/**
 * @defgroup   cpp
 * @brief      CPP module.
 * @{
 */
#ifndef CPP_H
#define CPP_H

#include "uint64.h"
#include "bool.h"
#include "hashmap.h"
#include "strarray.h"
#include "buffer.h"
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cpp_token cpp_token;
typedef struct cpp_file cpp_file;
typedef struct cpp_macro_param cpp_macro_param;
typedef struct cpp_macro_arg cpp_macro_arg;
typedef struct cpp_macro cpp_macro;
typedef struct cpp_cond_incl cpp_cond_incl;
typedef struct cpp_hideset cpp_hideset;
typedef struct cpp_type cpp_type;
typedef struct cpp_node cpp_node;
typedef struct cpp_obj cpp_obj;
typedef struct cpp_scope cpp_scope;

/* cpp_token*/
typedef enum {
  TK_IDENT,   /* Identifiers */
  TK_PUNCT,   /* Punctuators */
  TK_KEYWORD, /* Keywords */
  TK_STR,     /* String literals */
  TK_NUM,     /* Numeric literals */
  TK_PP_NUM,  /* Preprocessing numbers */
  TK_EOF,     /* End-of-file markers */
} cpp_token_kind;

typedef enum {
  STR_NONE,
  STR_UTF8,
  STR_UTF16,
  STR_UTF32,
  STR_WIDE,
} cpp_string_kind;

typedef enum {
  TY_VOID,
  TY_BOOL,
  TY_CHAR,
  TY_SHORT,
  TY_INT,
  TY_LONG,
  TY_FLOAT,
  TY_DOUBLE,
  TY_LDOUBLE,
  TY_ENUM,
  TY_PTR,
  TY_FUNC,
  TY_ARRAY,
  TY_VLA, /* variable-length array */
  TY_STRUCT,
  TY_UNION,
} cpp_type_kind;

int64 cpp_const_expr(cpp_token**, cpp_token*);
bool cpp_convert_int(cpp_token*);
cpp_token* cpp_copy_line(cpp_token**, cpp_token*);
char* cpp_detect_include_guard(cpp_token*);
void cpp_error_at(char*, char*, ...);
void cpp_error_tok(cpp_token*, char*, ...);
void cpp_warn_tok(cpp_token*, char*, ...);
long cpp_eval_const_expr(cpp_token**, cpp_token*);
cpp_file* cpp_file_new(const char*, int, char*);
void cpp_file_free(cpp_file*);
cpp_hideset* cpp_hideset_new(char*);
void cpp_hideset_free(cpp_hideset*);
cpp_token* cpp_hideset_add(cpp_token*, cpp_hideset*);
bool cpp_hideset_contains(cpp_hideset*, char*, int);
cpp_hideset* cpp_hideset_intersection(cpp_hideset*, cpp_hideset*);
cpp_hideset* cpp_hideset_union(cpp_hideset*, cpp_hideset*);
void cpp_init_macros(void);
void cpp_define_gnu_macros(void);
void cpp_join_adjacent_string_literals(cpp_token*);
char* cpp_join_tokens(cpp_token*, cpp_token*);
cpp_macro* cpp_macro_add(char*, bool, cpp_token*);
void cpp_macro_define(char*, char*);
bool cpp_macro_expand(cpp_token**, cpp_token*);
cpp_macro* cpp_macro_find(cpp_token*);
void cpp_macro_undef(char*);
cpp_token* cpp_new_num_token(int, cpp_token*);
cpp_token* cpp_new_str_token(char*, cpp_token*);
cpp_token* cpp_paste(cpp_token*, cpp_token*);
cpp_token* cpp_preprocess2(cpp_token*);
cpp_token* cpp_preprocess(cpp_token*);
void cpp_print_tokens(buffer*, cpp_token*, bool);
char* cpp_quote_string(char*);
cpp_token* cpp_read_const_expr(cpp_token**, cpp_token*);
char* cpp_read_file(char*);
char* cpp_read_include_filename(cpp_token**, cpp_token*, bool*);
void cpp_read_line_marker(cpp_token**, cpp_token*);
cpp_macro_arg* cpp_read_macro_arg_one(cpp_token**, cpp_token*, bool);
cpp_macro_arg* cpp_read_macro_args(cpp_token**, cpp_token*, cpp_macro_param*, char*);
void cpp_read_macro_definition(cpp_token**, cpp_token*);
cpp_macro_param* cpp_read_macro_params(cpp_token**, cpp_token*, char**);
char* cpp_search_include_next(char*);
char* cpp_search_include_paths(char*);
cpp_token* cpp_skip_cond_incl2(cpp_token*);
cpp_token* cpp_skip_cond_incl(cpp_token*);
cpp_token* cpp_skip_line(cpp_token*);
cpp_token* cpp_stringize(cpp_token*, cpp_token*);
cpp_token* cpp_subst(cpp_token*, cpp_macro_arg*);
cpp_token* cpp_token_new(cpp_token_kind, char*, char*);
cpp_token* cpp_token_eof(cpp_token*);
cpp_token* cpp_token_free(cpp_token*);
cpp_token* cpp_token_copy(cpp_token*);
cpp_token* cpp_token_append(cpp_token*, cpp_token*);
void cpp_token_dump(buffer*, cpp_token*);
cpp_token* cpp_tokenize(cpp_file*);
cpp_token* cpp_tokenize_file(char*);
cpp_type* cpp_type_new(cpp_type_kind, int, int);
cpp_type* cpp_type_copy(cpp_type*);
void cpp_type_free(cpp_type*);
void cpp_convert_number(cpp_token*);
void cpp_convert_tokens(cpp_token*);

#ifdef __cplusplus
}
#endif

#endif /* defined CPP_H */
/** @} */
