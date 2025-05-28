#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"
#include "../strarray.h"
#include "../path.h"
#include <string.h>
#include <errno.h>

  
static cpp_token* include_file(cpp_token* tok, char* path, cpp_token* filename_tok);

static cpp_cond_incl*
push_cond_incl(cpp_token* tok, bool included) {
  cpp_cond_incl* ci;

  if((ci = alloc_zero(sizeof(cpp_cond_incl)))) {
    ci->next = cond_incl;
    ci->ctx = IN_THEN;
    ci->tok = tok;
    ci->included = included;
  }

  cond_incl = ci;

  return ci;
}

static void
pop_cond_incl(void) {
  cpp_cond_incl* ci = cond_incl;

  alloc_free(ci);

  cond_incl = cond_incl->next;
}

cpp_token*
cpp_preprocess2(cpp_token* tok) {
  cpp_token head = {};
  cpp_token* cur = &head;

  while(tok->kind != TK_EOF) {
    /* If it is a macro, expand it. */
    if(cpp_expand(&tok, tok))
      continue;

    /* Pass through if it is not a "#". */
    if(!cpp_is_hash(tok)) {
      tok->line_delta = tok->file->line_delta;
      tok->filename = tok->file->display_name;
      cur = cur->next = tok;
      tok = tok->next;
      continue;
    }

    cpp_token* start = tok;
    tok = tok->next;

    if(cpp_equal(tok, "include")) {
      bool is_dquote;
      char* filename = cpp_read_include_filename(&tok, tok->next, &is_dquote);

      if(filename[0] != '/' && is_dquote) {
        stralloc dir;
        stralloc_init(&dir);
        path_dirname(start->file->name, &dir);
        stralloc_nul(&dir);
        char* path = cpp_format("%s/%s", dir.s, filename);
        stralloc_free(&dir);

        if(path_exists(path)) {
          path[path_collapse(path, str_len(path))] = '\0';

          tok = include_file(tok, path, start->next->next);
          continue;
        }
      }

      char* path = cpp_search_include_paths(filename);
      tok = include_file(tok, path ? path : filename, start->next->next);
      continue;
    }

    if(cpp_equal(tok, "include_next")) {
      bool ignore;
      char* filename = cpp_read_include_filename(&tok, tok->next, &ignore);
      char* path = cpp_search_include_next(filename);
      tok = include_file(tok, path ? path : filename, start->next->next);
      continue;
    }

    if(cpp_equal(tok, "define")) {
      cpp_read_macro_definition(&tok, tok->next);
      continue;
    }

    if(cpp_equal(tok, "undef")) {
      tok = tok->next;

      if(tok->kind != TK_IDENT)
        cpp_error_tok(tok, "macro name must be an identifier");

      cpp_undefine(str_ndup(tok->loc, tok->len));
      tok = cpp_skip_line(tok->next);
      continue;
    }

    if(cpp_equal(tok, "if")) {
      long val = cpp_eval_const_expr(&tok, tok);

      push_cond_incl(start, val);

      if(!val)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "ifdef")) {
      bool defined = cpp_macro_find(tok->next);

      push_cond_incl(tok, defined);
      tok = cpp_skip_line(tok->next->next);

      if(!defined)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "ifndef")) {
      bool defined = cpp_macro_find(tok->next);

      push_cond_incl(tok, !defined);
      tok = cpp_skip_line(tok->next->next);

      if(defined)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "elif")) {
      if(!cond_incl || cond_incl->ctx == IN_ELSE)
        cpp_error_tok(start, "stray #elif");

      cond_incl->ctx = IN_ELIF;

      if(!cond_incl->included && cpp_eval_const_expr(&tok, tok))
        cond_incl->included = true;
      else
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "else")) {
      if(!cond_incl || cond_incl->ctx == IN_ELSE)
        cpp_error_tok(start, "stray #else");

      cond_incl->ctx = IN_ELSE;
      tok = cpp_skip_line(tok->next);

      if(cond_incl->included)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "endif")) {
      if(!cond_incl)
        cpp_error_tok(start, "stray #endif");

      cond_incl = cond_incl->next;
      //  pop_cond_incl();
      tok = cpp_skip_line(tok->next);
      continue;
    }

    if(cpp_equal(tok, "line")) {
      cpp_read_line_marker(&tok, tok->next);
      continue;
    }

    if(tok->kind == TK_PP_NUM) {
      cpp_read_line_marker(&tok, tok);
      continue;
    }

    if(cpp_equal(tok, "pragma") && cpp_equal(tok->next, "once")) {
      hashmap_put(&cpp_pragma_once, tok->file->name, (void*)1);
      tok = cpp_skip_line(tok->next->next);
      continue;
    }

    if(cpp_equal(tok, "pragma")) {
      do
        tok = tok->next;
      while(!tok->at_bol);

      continue;
    }

    if(cpp_equal(tok, "error"))
      cpp_error_tok(tok, "error");

    if(cpp_equal(tok, "warning")) {
      cpp_warn_tok(tok, "warning");
      continue;
    }

    /* `#`-only line is legal. It's called a null directive. */
    if(tok->at_bol)
      continue;

    cpp_error_tok(tok, "invalid preprocessor directive");
  }

  cur->next = tok;
  return head.next;
}

static cpp_token*
include_file(cpp_token* tok, char* path, cpp_token* filename_tok) {
  cpp_token* tok2;
  char* guard_name;

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "include_file '", path, "'", tok->file ? " from '" : 0, tok->file ? tok->file->name : 0, "'", 0);
  buffer_putnlflush(buffer_2);
#endif

  /* Check for "#pragma once" */
  if(hashmap_get(&cpp_pragma_once, path))
    return tok;

  /* If we read the same file before, and if the file was guarded
     by the usual #ifndef ... #endif pattern, we may be able to
     cpp_skip the file without opening it. */
  if((guard_name = hashmap_get(&include_guards, path)) && hashmap_get(&cpp_macros, guard_name))
    return tok;

  if(!(tok2 = cpp_tokenize_file(path)))
    cpp_error_tok(filename_tok, "%s: cannot open file: %s", path, strerror(errno));

  if(!(ptrdiff_t)hashmap_get(&include_list, path)) {
    hashmap_put(&include_list, path, (void*)1);
    strarray_push(&include_array, path);
  }

  if((guard_name = cpp_detect_include_guard(tok2)))
    hashmap_put(&include_guards, path, guard_name);

  return cpp_token_append(tok2, tok);
}
