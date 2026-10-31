#include "../cpp.h"
#include "../cpp_internal.h"
#include "../str.h"

void
cpp_define(cpp_ctx* pp, char* name, char* buf) {
  size_t nlen = str_len(name), blen = str_len(buf);
  char* b = alloc(nlen + 1 + blen + 1);
  cpp_file* file;
  cpp_token *tok, *tok2 = 0;

  str_copy(b, name);
  b[nlen] = ' ';
  str_copy(&b[nlen + 1], buf);

  file = cpp_file_new("<built-in>", 1, b);
  tok = cpp_tokenize(pp, file);

  cpp_read_macro_definition(pp, &tok2, tok);

  while(tok)
    tok = cpp_token_free(tok);

  cpp_file_free(file);
}
