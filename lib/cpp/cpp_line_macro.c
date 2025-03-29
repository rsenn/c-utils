#include "../cpp.h"
#include "../cpp_internal.h"

cpp_token*
cpp_line_macro(cpp_token* tmpl) {
  while(tmpl->origin)
    tmpl = tmpl->origin;

  int i = tmpl->line_no + tmpl->file->line_delta;
  return cpp_new_num_token(i, tmpl);
}
