#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token *cpp_new_str_token(char *str, cpp_token *tmpl) {
  char *buf = cpp_quote_string(str);
  return tokenize(cpp_new_file(tmpl->file->name, tmpl->file->file_no, buf));
}
