#include "../stralloc.h"
#include "../json_internal.h"

static const char identifier_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$_";

static int
is_identifier_char(int c, size_t pos, void* ptr) {
  return !!identifier_chars[str_chr(identifier_chars, c)];
}

int
json_null_parse(jsonval* j, charbuf* b) {
  stralloc tok;
  stralloc_init(&tok);

  charbuf_get_pred(b, &tok, is_identifier_char, 0);

  if(tok.len == 4 && byte_equal(tok.s, 4, "null")) {
    charbuf_skipn(b, 4);
    j->type = JSON_NULL;
    return 1;
  }

  return 0;
}