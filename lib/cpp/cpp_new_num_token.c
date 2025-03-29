#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_new_num_token(int val, cpp_token* tmpl) {
  char* buf = cpp_format("%d\n", val);
  cpp_file* file = cpp_file_new(tmpl->file->name, tmpl->file->file_no, buf);
  cpp_token* tok = cpp_tokenize(file);
  cpp_file_free(file);
  return tok;
}
