#include "../cpp_internal.h"
#include "../alloc.h"

/* Create a new token. */
cpp_token*
cpp_token_new(cpp_token_kind kind, char* start, char* end) {
  cpp_token* tok;

  if((tok = alloc_zero(sizeof(cpp_token)))) {
    tok->kind = kind;
    tok->loc = start;
    tok->len = end - start;
    tok->file = cpp_current_file ? cpp_file_dup(cpp_current_file) : 0;
    tok->filename = cpp_current_file ? cpp_current_file->display_name : 0;
    tok->at_bol = cpp_at_bol;
    tok->has_space = cpp_has_space;
  }

  cpp_at_bol = cpp_has_space = false;
  return tok;
}

cpp_token*
cpp_token_free(cpp_token* tok) {
  cpp_token* next = tok->next;

  if(tok->file)
    cpp_file_free(tok->file);

  alloc_free(tok);
  return next;
}

cpp_token*
cpp_token_copy(cpp_token* tok) {
  cpp_token* t;

  if((t = alloc_zero(sizeof(cpp_token)))) {
    *t = *tok;

    if(t->file)
      cpp_file_dup(t->file);

    t->next = NULL;
  }

  return t;
}

void
cpp_token_dump(buffer* out, cpp_token* tok) {
  if(tok->file) {
    buffer_puts(out, "file: ");
    buffer_putspad(out, tok->file->name, 20);
  }

  if(tok->file) {
    size_t pos = tok->loc - tok->file->contents;

    buffer_puts(out, "  pos: ");
    buffer_putulonglongpad(out, pos, 5);
  }

  buffer_puts(out, "  kind: ");
  buffer_puts(out, ((const char*[]){"IDENT  ", "PUNCT  ", "KEYWORD", "STR    ", "NUM    ", "PP_NUM ", "EOF    "}[tok->kind]));

  buffer_puts(out, "  at_bol=");
  buffer_putulong(out, tok->at_bol);

  buffer_puts(out, "  str: '");
  buffer_put(out, tok->loc, tok->len);
  buffer_puts(out, "'");

  buffer_putnlflush(out);
}
