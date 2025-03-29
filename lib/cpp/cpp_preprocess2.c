#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"
#include "../path.h"

cpp_token*
cpp_preprocess2(cpp_token* tok) {
  cpp_token head = {};
  cpp_token* cur = &head;

  while(tok->kind != TK_EOF) {
    /* If it is a macro, expand it. */
    if(cpp_macro_expand(&tok, tok))
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
        char* path = cpp_format("%.*s/%s", (int)path_dirlen(start->file->name), start->file->name, filename);

        if(path_exists(path)) {
          tok = cpp_include_file(tok, path, start->next->next);
          continue;
        }
      }

      char* path = cpp_search_include_paths(filename);
      tok = cpp_include_file(tok, path ? path : filename, start->next->next);
      continue;
    }

    if(cpp_equal(tok, "include_next")) {
      bool ignore;
      char* filename = cpp_read_include_filename(&tok, tok->next, &ignore);
      char* path = cpp_search_include_next(filename);
      tok = cpp_include_file(tok, path ? path : filename, start->next->next);
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

      cpp_macro_undef(str_ndup(tok->loc, tok->len));
      tok = cpp_skip_line(tok->next);
      continue;
    }

    if(cpp_equal(tok, "if")) {
      long val = cpp_eval_const_expr(&tok, tok);

      cpp_push_cond_incl(start, val);

      if(!val)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "ifdef")) {
      bool defined = cpp_macro_find(tok->next);

      cpp_push_cond_incl(tok, defined);
      tok = cpp_skip_line(tok->next->next);

      if(!defined)
        tok = cpp_skip_cond_incl(tok);

      continue;
    }

    if(cpp_equal(tok, "ifndef")) {
      bool defined = cpp_macro_find(tok->next);

      cpp_push_cond_incl(tok, !defined);
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
