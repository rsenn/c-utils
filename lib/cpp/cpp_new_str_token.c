#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_new_str_token(char* str, cpp_token* tmpl) {
  char* buf = cpp_quote_string(str);

  cpp_token* tok = cpp_tokenize(cpp_file_new(tmpl->file->name, tmpl->file->file_no, buf));

  cpp_file_free(cpp_current_file);
  cpp_current_file = 0;

  return tok;
}
