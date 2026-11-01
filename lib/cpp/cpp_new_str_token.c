#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_new_str_token(cpp_ctx* pp, const char* str, cpp_token* tmpl) {
  char* buf = cpp_quote_string(str);
  cpp_file* file = cpp_file_new(tmpl->file->name, tmpl->file->file_no, buf);
  cpp_token* tok = cpp_tokenize(pp, file);
  cpp_file_free(file);
  /* the new token takes the place of tmpl: it is not the start of a line of its own
   * (it printed as a line break and a line marker in the middle of a line) */
  if(tok) {
    tok->at_bol = tmpl->at_bol;
    tok->has_space = tmpl->has_space;
  }
  return tok;
}
