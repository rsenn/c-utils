#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_new_str_token(char* str, cpp_token* tmpl) {
  char* buf = cpp_quote_string(str);
  cpp_file* file = cpp_file_new(tmpl->file->name, tmpl->file->file_no, buf);
  cpp_token* tok = cpp_tokenize(file);
  cpp_file_free(file);
  return tok;
}
