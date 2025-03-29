#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_file_macro(cpp_token* tmpl) {
  while(tmpl->origin)
    tmpl = tmpl->origin;
  return cpp_new_str_token(tmpl->file->display_name, tmpl);
}
