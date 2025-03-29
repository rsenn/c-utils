#include "../cpp.h"
#include "../cpp_internal.h"

/* __COUNTER__ is expanded to serial values starting from 0. */
cpp_token*
cpp_counter_macro(cpp_token* tmpl) {
  static int i = 0;
  return cpp_new_num_token(i++, tmpl);
}
