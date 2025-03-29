#include "../cpp.h"
#include "../cpp_internal.h"

void cpp_init_macros(void) {
  // Define predefined macros
  cpp_define_macro("_LP64", "1");
  cpp_define_macro("__C99_MACRO_WITH_VA_ARGS", "1");
  cpp_define_macro("__ELF__", "1");
  cpp_define_macro("__LP64__", "1");
  cpp_define_macro("__SIZEOF_DOUBLE__", "8");
  cpp_define_macro("__SIZEOF_FLOAT__", "4");
  cpp_define_macro("__SIZEOF_INT__", "4");
  cpp_define_macro("__SIZEOF_LONG_DOUBLE__", "8");
  cpp_define_macro("__SIZEOF_LONG_LONG__", "8");
  cpp_define_macro("__SIZEOF_LONG__", "8");
  cpp_define_macro("__SIZEOF_POINTER__", "8");
  cpp_define_macro("__SIZEOF_PTRDIFF_T__", "8");
  cpp_define_macro("__SIZEOF_SHORT__", "2");
  cpp_define_macro("__SIZEOF_SIZE_T__", "8");
  cpp_define_macro("__SIZE_TYPE__", "unsigned long");
  cpp_define_macro("__STDC_HOSTED__", "1");
  cpp_define_macro("__STDC_NO_COMPLEX__", "1");
  cpp_define_macro("__STDC_UTF_16__", "1");
  cpp_define_macro("__STDC_UTF_32__", "1");
  cpp_define_macro("__STDC_VERSION__", "201112L");
  cpp_define_macro("__STDC__", "1");
  cpp_define_macro("__USER_LABEL_PREFIX__", "");
  cpp_define_macro("__alignof__", "_Alignof");
  cpp_define_macro("__amd64", "1");
  cpp_define_macro("__amd64__", "1");
  cpp_define_macro("__chibicc__", "1");
  cpp_define_macro("__const__", "const");
  cpp_define_macro("__gnu_linux__", "1");
  cpp_define_macro("__inline__", "inline");
  cpp_define_macro("__linux", "1");
  cpp_define_macro("__linux__", "1");
  cpp_define_macro("__signed__", "signed");
  cpp_define_macro("__typeof__", "typeof");
  cpp_define_macro("__unix", "1");
  cpp_define_macro("__unix__", "1");
  cpp_define_macro("__volatile__", "volatile");
  cpp_define_macro("__x86_64", "1");
  cpp_define_macro("__x86_64__", "1");
  cpp_define_macro("linux", "1");
  cpp_define_macro("unix", "1");

  cpp_add_builtin("__FILE__", cpp_file_macro);
  cpp_add_builtin("__LINE__", cpp_line_macro);
  cpp_add_builtin("__COUNTER__", cpp_counter_macro);
  cpp_add_builtin("__TIMESTAMP__", cpp_timestamp_macro);
  cpp_add_builtin("__BASE_FILE__", cpp_base_file_macro);

  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  cpp_define_macro("__DATE__", cpp_format_date(tm));
  cpp_define_macro("__TIME__", cpp_format_time(tm));
}
