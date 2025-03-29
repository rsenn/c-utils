#include "../cpp.h"
#include "../cpp_internal.h"

void cpp_define_macro(char *name, char *buf) {
  cpp_token *tok = tokenize(cpp_new_file("<built-in>", 1, buf));
  cpp_add_macro(name, true, tok);
}
