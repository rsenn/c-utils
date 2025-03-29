#include "../cpp.h"
#include "../cpp_internal.h"

strarray cpp_include_paths = { 0 };

void
cpp_define_macro(char* name, char* buf) {
  cpp_token* tok = cpp_tokenize(cpp_new_file("<built-in>", 1, buf));
  cpp_add_macro(name, true, tok);
}
