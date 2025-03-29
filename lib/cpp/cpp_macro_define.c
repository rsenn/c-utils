#include "../cpp.h"
#include "../cpp_internal.h"
#include "../alloc.h"

void
cpp_macro_define(char* name, char* buf) {
  cpp_file* file = cpp_file_new("<built-in>", 1, buf);
  cpp_token* tok = cpp_tokenize(file);

  cpp_macro_add(name, true, tok);

  /* while(tok)
     tok = cpp_token_free(tok);*/

  cpp_file_free(file);
  /*  cpp_current_file = 0;*/
}
