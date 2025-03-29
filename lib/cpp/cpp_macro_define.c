#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

void
cpp_macro_define(char* name, char* buf) {
  size_t nlen = str_len(name), blen = str_len(buf);
  char* b = alloca(nlen + 1 + blen + 1);

  str_copy(b, name);
  b[nlen] = ' ';
  str_copy(&b[nlen + 1], buf);

  cpp_file* file = cpp_file_new("<built-in>", 1, b);
  cpp_token *tok = cpp_tokenize(file), *tok2 = 0;

  cpp_read_macro_definition(&tok2, tok);

  while(tok)
    tok = cpp_token_free(tok);

  cpp_file_free(file);
}
