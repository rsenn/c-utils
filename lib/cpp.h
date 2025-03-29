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
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cpp_token cpp_token;
typedef struct cpp_file cpp_file;
typedef struct cpp_macro_param cpp_macro_param;
typedef struct cpp_macro_arg cpp_macro_arg;
typedef cpp_token* macro_handler_fn(cpp_token*);
typedef struct cpp_macro cpp_macro;
typedef struct cpp_cond_incl cpp_cond_incl;
typedef struct cpp_hideset cpp_hideset;
typedef struct cpp_type cpp_type;
typedef struct cpp_node cpp_node;

typedef enum {
  STR_NONE,
  STR_UTF8,
  STR_UTF16,
  STR_UTF32,
  STR_WIDE,
} cpp_string_kind;

cpp_macro* cpp_add_builtin(char*, macro_handler_fn*);
cpp_token* cpp_add_hideset(cpp_token*, cpp_hideset*);
cpp_macro* cpp_add_macro(char*, _Bool, cpp_token*);
cpp_token* cpp_append(cpp_token*, cpp_token*);
cpp_token* cpp_base_file_macro(cpp_token*);
cpp_token* cpp_copy_line(cpp_token**, cpp_token*);
cpp_token* cpp_copy_token(cpp_token*);
cpp_token* cpp_counter_macro(cpp_token*);
void cpp_define_macro(char*, char*);
char* cpp_detect_include_guard(cpp_token*);
long cpp_eval_const_expr(cpp_token**, cpp_token*);
_Bool cpp_expand_macro(cpp_token**, cpp_token*);
cpp_token* cpp_file_macro(cpp_token*);
cpp_macro_arg* cpp_find_arg(cpp_macro_arg*, cpp_token*);
cpp_macro* cpp_find_macro(cpp_token*);
char* cpp_format_date(struct tm*);
char* cpp_format_time(struct tm*);
cpp_string_kind cpp_get_string_kind(cpp_token*);
_Bool cpp_has_varargs(cpp_macro_arg*);
_Bool cpp_hideset_contains(cpp_hideset*, char*, int);
cpp_hideset* cpp_hideset_intersection(cpp_hideset*, cpp_hideset*);
cpp_hideset* cpp_hideset_union(cpp_hideset*, cpp_hideset*);
cpp_token* cpp_include_file(cpp_token*, char*, cpp_token*);
void cpp_init_macros(void);
_Bool cpp_is_hash(cpp_token*);
void cpp_join_adjacent_string_literals(cpp_token*);
char* cpp_join_tokens(cpp_token*, cpp_token*);
cpp_token* cpp_line_macro(cpp_token*);
cpp_token* cpp_new_eof(cpp_token*);
cpp_file* cpp_new_file(char*, int, char*);
cpp_hideset* cpp_new_hideset(char*);
cpp_token* cpp_new_num_token(int, cpp_token*);
cpp_token* cpp_new_str_token(char*, cpp_token*);
cpp_token* cpp_paste(cpp_token*, cpp_token*);
char* cpp_quote_string(char*);
cpp_token* cpp_read_const_expr(cpp_token**, cpp_token*);
char* cpp_read_include_filename(cpp_token**, cpp_token*, _Bool*);
void cpp_read_line_marker(cpp_token**, cpp_token*);
cpp_macro_arg* cpp_read_macro_arg_one(cpp_token**, cpp_token*, _Bool);
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
cpp_token* cpp_timestamp_macro(cpp_token*);
void cpp_undef_macro(char*);
void cpp_error_tok(cpp_token*, char*, ...);
void cpp_error_at(char* loc, char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* defined CPP_H */
/** @} */
