#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_base_file_macro(cpp_token* tmpl) {
  return cpp_new_str_token(cpp_base_file, tmpl);
}
