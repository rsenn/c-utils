#include "../cpp.h"
#include "../cpp_internal.h"

// Read #line arguments
void
cpp_read_line_marker(cpp_token** rest, cpp_token* tok) {
  cpp_token* start = tok;
  tok = cpp_preprocess(cpp_copy_line(rest, tok));

  if(tok->kind != TK_NUM || tok->ty->kind != TY_INT)
    cpp_error_tok(tok, "invalid line marker");
  start->file->line_delta = tok->val - start->line_no;

  tok = tok->next;
  if(tok->kind == TK_EOF)
    return;

  if(tok->kind != TK_STR)
    cpp_error_tok(tok, "filename expected");
  start->file->display_name = tok->str;
}
