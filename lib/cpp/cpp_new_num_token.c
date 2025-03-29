#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token *cpp_new_num_token(int val, cpp_token *tmpl) {
  char *buf = cpp_format("%d\n", val);

  return cpp_tokenize(cpp_new_file(tmpl->file->name, tmpl->file->file_no, buf));
}
