#include "../cpp.h"
#include "../cpp_internal.h"
#include <sys/stat.h>

char* cpp_base_file = 0;

static char* format_time(struct tm* tm);
static char* format_date(struct tm* tm);

static void define_gnu_macros(void);

static cpp_token*
file_macro(cpp_token* tmpl) {
  while(tmpl->origin)
    tmpl = tmpl->origin;

  return cpp_new_str_token(tmpl->file->display_name, tmpl);
}

static cpp_token*
line_macro(cpp_token* tmpl) {
  while(tmpl->origin)
    tmpl = tmpl->origin;

  int i = tmpl->line_no + tmpl->file->line_delta;
  return cpp_new_num_token(i, tmpl);
}

/* __COUNTER__ is expanded to serial values starting from 0. */
static cpp_token*
counter_macro(cpp_token* tmpl) {
  static int i = 0;
  return cpp_new_num_token(i++, tmpl);
}

/* "Fri Jul 24 01:32:50 2020" */
static cpp_token*
timestamp_macro(cpp_token* tmpl) {
  struct stat st;

  if(stat(tmpl->file->name, &st) != 0)
    return cpp_new_str_token("??? ??? ?? ??:??:?? ????", tmpl);

  char buf[30];
  ctime_r(&st.st_mtime, buf);
  buf[24] = '\0';
  return cpp_new_str_token(buf, tmpl);
}

static cpp_token*
base_file_macro(cpp_token* tmpl) {
  return cpp_new_str_token(cpp_base_file, tmpl);
}

void
cpp_init_macros(void) {
  /* Define predefined cpp_macros */
  /*cpp_macro_define("_LP64", "1");
  cpp_macro_define("__C99_MACRO_WITH_VA_ARGS", "1");
  cpp_macro_define("__ELF__", "1");
  cpp_macro_define("__LP64__", "1");
  cpp_macro_define("__SIZEOF_DOUBLE__", "8");
  cpp_macro_define("__SIZEOF_FLOAT__", "4");
  cpp_macro_define("__SIZEOF_INT__", "4");
  cpp_macro_define("__SIZEOF_LONG_DOUBLE__", "8");
  cpp_macro_define("__SIZEOF_LONG_LONG__", "8");
  cpp_macro_define("__SIZEOF_LONG__", "8");
  cpp_macro_define("__SIZEOF_POINTER__", "8");
  cpp_macro_define("__SIZEOF_PTRDIFF_T__", "8");
  cpp_macro_define("__SIZEOF_SHORT__", "2");
  cpp_macro_define("__SIZEOF_SIZE_T__", "8");
  cpp_macro_define("__SIZE_TYPE__", "unsigned long");
  cpp_macro_define("__STDC_HOSTED__", "1");
  cpp_macro_define("__STDC_NO_COMPLEX__", "1");
  cpp_macro_define("__STDC_UTF_16__", "1");
  cpp_macro_define("__STDC_UTF_32__", "1");
  cpp_macro_define("__STDC_VERSION__", "201112L");
  cpp_macro_define("__STDC__", "1");
  cpp_macro_define("__USER_LABEL_PREFIX__", "");
  cpp_macro_define("__alignof__", "_Alignof");
  cpp_macro_define("__amd64", "1");
  cpp_macro_define("__amd64__", "1");
  cpp_macro_define("__chibicc__", "1");
  cpp_macro_define("__const__", "const");
  cpp_macro_define("__gnu_linux__", "1");
  cpp_macro_define("__inline__", "inline");
  cpp_macro_define("__linux", "1");
  cpp_macro_define("__linux__", "1");
  cpp_macro_define("__signed__", "signed");
  cpp_macro_define("__typeof__", "typeof");
  cpp_macro_define("__unix", "1");
  cpp_macro_define("__unix__", "1");
  cpp_macro_define("__volatile__", "volatile");
  cpp_macro_define("__x86_64", "1");
  cpp_macro_define("__x86_64__", "1");
  cpp_macro_define("linux", "1");
  cpp_macro_define("unix", "1");*/

  define_gnu_macros();

  cpp_add_builtin("__FILE__", file_macro);
  cpp_add_builtin("__LINE__", line_macro);
  cpp_add_builtin("__COUNTER__", counter_macro);
  cpp_add_builtin("__TIMESTAMP__", timestamp_macro);
  cpp_add_builtin("__BASE_FILE__", base_file_macro);

  {
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);
    cpp_macro_define("__DATE__", format_date(tm));
    cpp_macro_define("__TIME__", format_time(tm));
  }
}

static void
define_gnu_macros(void) {
  cpp_macro_define("__amd64", "1");
  cpp_macro_define("__amd64__", "1");
  cpp_macro_define("__ATOMIC_ACQ_REL", "4");
  cpp_macro_define("__ATOMIC_ACQUIRE", "2");
  cpp_macro_define("__ATOMIC_CONSUME", "1");
  cpp_macro_define("__ATOMIC_HLE_ACQUIRE", "65536");
  cpp_macro_define("__ATOMIC_HLE_RELEASE", "131072");
  cpp_macro_define("__ATOMIC_RELAXED", "0");
  cpp_macro_define("__ATOMIC_RELEASE", "3");
  cpp_macro_define("__ATOMIC_SEQ_CST", "5");
  cpp_macro_define("__BFLT16_DECIMAL_DIG__", "4");
  cpp_macro_define("__BFLT16_DENORM_MIN__", "9.18354961579912115600575419704879436e-41BF16");
  cpp_macro_define("__BFLT16_DIG__", "2");
  cpp_macro_define("__BFLT16_EPSILON__", "7.81250000000000000000000000000000000e-3BF16");
  cpp_macro_define("__BFLT16_HAS_DENORM__", "1");
  cpp_macro_define("__BFLT16_HAS_INFINITY__", "1");
  cpp_macro_define("__BFLT16_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__BFLT16_IS_IEC_60559__", "0");
  cpp_macro_define("__BFLT16_MANT_DIG__", "8");
  cpp_macro_define("__BFLT16_MAX_10_EXP__", "38");
  cpp_macro_define("__BFLT16_MAX__", "3.38953138925153547590470800371487867e+38BF16");
  cpp_macro_define("__BFLT16_MAX_EXP__", "128");
  cpp_macro_define("__BFLT16_MIN_10_EXP__", "(-37)");
  cpp_macro_define("__BFLT16_MIN__", "1.17549435082228750796873653722224568e-38BF16");
  cpp_macro_define("__BFLT16_MIN_EXP__", "(-125)");
  cpp_macro_define("__BFLT16_NORM_MAX__", "3.38953138925153547590470800371487867e+38BF16");
  cpp_macro_define("__BIGGEST_ALIGNMENT__", "16");
  cpp_macro_define("__BYTE_ORDER__", "__ORDER_LITTLE_ENDIAN__");
  cpp_macro_define("__CET__", "3");
  cpp_macro_define("__CHAR16_TYPE__", "short unsigned int");
  cpp_macro_define("__CHAR32_TYPE__", "unsigned int");
  cpp_macro_define("__CHAR_BIT__", "8");
  cpp_macro_define("__code_model_small__", "1");
  cpp_macro_define("__DBL_DECIMAL_DIG__", "17");
  cpp_macro_define("__DBL_DENORM_MIN__", "((double)4.94065645841246544176568792868221372e-324L)");
  cpp_macro_define("__DBL_DIG__", "15");
  cpp_macro_define("__DBL_EPSILON__", "((double)2.22044604925031308084726333618164062e-16L)");
  cpp_macro_define("__DBL_HAS_DENORM__", "1");
  cpp_macro_define("__DBL_HAS_INFINITY__", "1");
  cpp_macro_define("__DBL_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__DBL_IS_IEC_60559__", "1");
  cpp_macro_define("__DBL_MANT_DIG__", "53");
  cpp_macro_define("__DBL_MAX_10_EXP__", "308");
  cpp_macro_define("__DBL_MAX__", "((double)1.79769313486231570814527423731704357e+308L)");
  cpp_macro_define("__DBL_MAX_EXP__", "1024");
  cpp_macro_define("__DBL_MIN_10_EXP__", "(-307)");
  cpp_macro_define("__DBL_MIN__", "((double)2.22507385850720138309023271733240406e-308L)");
  cpp_macro_define("__DBL_MIN_EXP__", "(-1021)");
  cpp_macro_define("__DBL_NORM_MAX__", "((double)1.79769313486231570814527423731704357e+308L)");
  cpp_macro_define("__DEC128_EPSILON__", "1E-33DL");
  cpp_macro_define("__DEC128_MANT_DIG__", "34");
  cpp_macro_define("__DEC128_MAX__", "9.999999999999999999999999999999999E6144DL");
  cpp_macro_define("__DEC128_MAX_EXP__", "6145");
  cpp_macro_define("__DEC128_MIN__", "1E-6143DL");
  cpp_macro_define("__DEC128_MIN_EXP__", "(-6142)");
  cpp_macro_define("__DEC128_SUBNORMAL_MIN__", "0.000000000000000000000000000000001E-6143DL");
  cpp_macro_define("__DEC32_EPSILON__", "1E-6DF");
  cpp_macro_define("__DEC32_MANT_DIG__", "7");
  cpp_macro_define("__DEC32_MAX__", "9.999999E96DF");
  cpp_macro_define("__DEC32_MAX_EXP__", "97");
  cpp_macro_define("__DEC32_MIN__", "1E-95DF");
  cpp_macro_define("__DEC32_MIN_EXP__", "(-94)");
  cpp_macro_define("__DEC32_SUBNORMAL_MIN__", "0.000001E-95DF");
  cpp_macro_define("__DEC64_EPSILON__", "1E-15DD");
  cpp_macro_define("__DEC64_MANT_DIG__", "16");
  cpp_macro_define("__DEC64_MAX__", "9.999999999999999E384DD");
  cpp_macro_define("__DEC64_MAX_EXP__", "385");
  cpp_macro_define("__DEC64_MIN__", "1E-383DD");
  cpp_macro_define("__DEC64_MIN_EXP__", "(-382)");
  cpp_macro_define("__DEC64_SUBNORMAL_MIN__", "0.000000000000001E-383DD");
  cpp_macro_define("__DEC_EVAL_METHOD__", "2");
  cpp_macro_define("__DECIMAL_BID_FORMAT__", "1");
  cpp_macro_define("__DECIMAL_DIG__", "21");
  cpp_macro_define("__ELF__", "1");
  cpp_macro_define("__FINITE_MATH_ONLY__", "0");
  cpp_macro_define("__FLOAT_WORD_ORDER__", "__ORDER_LITTLE_ENDIAN__");
  cpp_macro_define("__FLT128_DECIMAL_DIG__", "36");
  cpp_macro_define("__FLT128_DENORM_MIN__", "6.47517511943802511092443895822764655e-4966F128");
  cpp_macro_define("__FLT128_DIG__", "33");
  cpp_macro_define("__FLT128_EPSILON__", "1.92592994438723585305597794258492732e-34F128");
  cpp_macro_define("__FLT128_HAS_DENORM__", "1");
  cpp_macro_define("__FLT128_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT128_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT128_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT128_MANT_DIG__", "113");
  cpp_macro_define("__FLT128_MAX_10_EXP__", "4932");
  cpp_macro_define("__FLT128_MAX__", "1.18973149535723176508575932662800702e+4932F128");
  cpp_macro_define("__FLT128_MAX_EXP__", "16384");
  cpp_macro_define("__FLT128_MIN_10_EXP__", "(-4931)");
  cpp_macro_define("__FLT128_MIN__", "3.36210314311209350626267781732175260e-4932F128");
  cpp_macro_define("__FLT128_MIN_EXP__", "(-16381)");
  cpp_macro_define("__FLT128_NORM_MAX__", "1.18973149535723176508575932662800702e+4932F128");
  cpp_macro_define("__FLT16_DECIMAL_DIG__", "5");
  cpp_macro_define("__FLT16_DENORM_MIN__", "5.96046447753906250000000000000000000e-8F16");
  cpp_macro_define("__FLT16_DIG__", "3");
  cpp_macro_define("__FLT16_EPSILON__", "9.76562500000000000000000000000000000e-4F16");
  cpp_macro_define("__FLT16_HAS_DENORM__", "1");
  cpp_macro_define("__FLT16_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT16_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT16_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT16_MANT_DIG__", "11");
  cpp_macro_define("__FLT16_MAX_10_EXP__", "4");
  cpp_macro_define("__FLT16_MAX__", "6.55040000000000000000000000000000000e+4F16");
  cpp_macro_define("__FLT16_MAX_EXP__", "16");
  cpp_macro_define("__FLT16_MIN_10_EXP__", "(-4)");
  cpp_macro_define("__FLT16_MIN__", "6.10351562500000000000000000000000000e-5F16");
  cpp_macro_define("__FLT16_MIN_EXP__", "(-13)");
  cpp_macro_define("__FLT16_NORM_MAX__", "6.55040000000000000000000000000000000e+4F16");
  cpp_macro_define("__FLT32_DECIMAL_DIG__", "9");
  cpp_macro_define("__FLT32_DENORM_MIN__", "1.40129846432481707092372958328991613e-45F32");
  cpp_macro_define("__FLT32_DIG__", "6");
  cpp_macro_define("__FLT32_EPSILON__", "1.19209289550781250000000000000000000e-7F32");
  cpp_macro_define("__FLT32_HAS_DENORM__", "1");
  cpp_macro_define("__FLT32_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT32_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT32_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT32_MANT_DIG__", "24");
  cpp_macro_define("__FLT32_MAX_10_EXP__", "38");
  cpp_macro_define("__FLT32_MAX__", "3.40282346638528859811704183484516925e+38F32");
  cpp_macro_define("__FLT32_MAX_EXP__", "128");
  cpp_macro_define("__FLT32_MIN_10_EXP__", "(-37)");
  cpp_macro_define("__FLT32_MIN__", "1.17549435082228750796873653722224568e-38F32");
  cpp_macro_define("__FLT32_MIN_EXP__", "(-125)");
  cpp_macro_define("__FLT32_NORM_MAX__", "3.40282346638528859811704183484516925e+38F32");
  cpp_macro_define("__FLT32X_DECIMAL_DIG__", "17");
  cpp_macro_define("__FLT32X_DENORM_MIN__", "4.94065645841246544176568792868221372e-324F32x");
  cpp_macro_define("__FLT32X_DIG__", "15");
  cpp_macro_define("__FLT32X_EPSILON__", "2.22044604925031308084726333618164062e-16F32x");
  cpp_macro_define("__FLT32X_HAS_DENORM__", "1");
  cpp_macro_define("__FLT32X_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT32X_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT32X_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT32X_MANT_DIG__", "53");
  cpp_macro_define("__FLT32X_MAX_10_EXP__", "308");
  cpp_macro_define("__FLT32X_MAX__", "1.79769313486231570814527423731704357e+308F32x");
  cpp_macro_define("__FLT32X_MAX_EXP__", "1024");
  cpp_macro_define("__FLT32X_MIN_10_EXP__", "(-307)");
  cpp_macro_define("__FLT32X_MIN__", "2.22507385850720138309023271733240406e-308F32x");
  cpp_macro_define("__FLT32X_MIN_EXP__", "(-1021)");
  cpp_macro_define("__FLT32X_NORM_MAX__", "1.79769313486231570814527423731704357e+308F32x");
  cpp_macro_define("__FLT64_DECIMAL_DIG__", "17");
  cpp_macro_define("__FLT64_DENORM_MIN__", "4.94065645841246544176568792868221372e-324F64");
  cpp_macro_define("__FLT64_DIG__", "15");
  cpp_macro_define("__FLT64_EPSILON__", "2.22044604925031308084726333618164062e-16F64");
  cpp_macro_define("__FLT64_HAS_DENORM__", "1");
  cpp_macro_define("__FLT64_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT64_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT64_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT64_MANT_DIG__", "53");
  cpp_macro_define("__FLT64_MAX_10_EXP__", "308");
  cpp_macro_define("__FLT64_MAX__", "1.79769313486231570814527423731704357e+308F64");
  cpp_macro_define("__FLT64_MAX_EXP__", "1024");
  cpp_macro_define("__FLT64_MIN_10_EXP__", "(-307)");
  cpp_macro_define("__FLT64_MIN__", "2.22507385850720138309023271733240406e-308F64");
  cpp_macro_define("__FLT64_MIN_EXP__", "(-1021)");
  cpp_macro_define("__FLT64_NORM_MAX__", "1.79769313486231570814527423731704357e+308F64");
  cpp_macro_define("__FLT64X_DECIMAL_DIG__", "21");
  cpp_macro_define("__FLT64X_DENORM_MIN__", "3.64519953188247460252840593361941982e-4951F64x");
  cpp_macro_define("__FLT64X_DIG__", "18");
  cpp_macro_define("__FLT64X_EPSILON__", "1.08420217248550443400745280086994171e-19F64x");
  cpp_macro_define("__FLT64X_HAS_DENORM__", "1");
  cpp_macro_define("__FLT64X_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT64X_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT64X_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT64X_MANT_DIG__", "64");
  cpp_macro_define("__FLT64X_MAX_10_EXP__", "4932");
  cpp_macro_define("__FLT64X_MAX__", "1.18973149535723176502126385303097021e+4932F64x");
  cpp_macro_define("__FLT64X_MAX_EXP__", "16384");
  cpp_macro_define("__FLT64X_MIN_10_EXP__", "(-4931)");
  cpp_macro_define("__FLT64X_MIN__", "3.36210314311209350626267781732175260e-4932F64x");
  cpp_macro_define("__FLT64X_MIN_EXP__", "(-16381)");
  cpp_macro_define("__FLT64X_NORM_MAX__", "1.18973149535723176502126385303097021e+4932F64x");
  cpp_macro_define("__FLT_DECIMAL_DIG__", "9");
  cpp_macro_define("__FLT_DENORM_MIN__", "1.40129846432481707092372958328991613e-45F");
  cpp_macro_define("__FLT_DIG__", "6");
  cpp_macro_define("__FLT_EPSILON__", "1.19209289550781250000000000000000000e-7F");
  cpp_macro_define("__FLT_EVAL_METHOD__", "0");
  cpp_macro_define("__FLT_EVAL_METHOD_TS_18661_3__", "0");
  cpp_macro_define("__FLT_HAS_DENORM__", "1");
  cpp_macro_define("__FLT_HAS_INFINITY__", "1");
  cpp_macro_define("__FLT_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__FLT_IS_IEC_60559__", "1");
  cpp_macro_define("__FLT_MANT_DIG__", "24");
  cpp_macro_define("__FLT_MAX_10_EXP__", "38");
  cpp_macro_define("__FLT_MAX__", "3.40282346638528859811704183484516925e+38F");
  cpp_macro_define("__FLT_MAX_EXP__", "128");
  cpp_macro_define("__FLT_MIN_10_EXP__", "(-37)");
  cpp_macro_define("__FLT_MIN__", "1.17549435082228750796873653722224568e-38F");
  cpp_macro_define("__FLT_MIN_EXP__", "(-125)");
  cpp_macro_define("__FLT_NORM_MAX__", "3.40282346638528859811704183484516925e+38F");
  cpp_macro_define("__FLT_RADIX__", "2");
  cpp_macro_define("__FXSR__", "1");
  cpp_macro_define("__GCC_ASM_FLAG_OUTPUTS__", "1");
  cpp_macro_define("__GCC_ATOMIC_BOOL_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_CHAR16_T_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_CHAR32_T_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_CHAR_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_INT_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_LLONG_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_LONG_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_POINTER_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_SHORT_LOCK_FREE", "2");
  cpp_macro_define("__GCC_ATOMIC_TEST_AND_SET_TRUEVAL", "1");
  cpp_macro_define("__GCC_ATOMIC_WCHAR_T_LOCK_FREE", "2");
  cpp_macro_define("__GCC_CONSTRUCTIVE_SIZE", "64");
  cpp_macro_define("__GCC_DESTRUCTIVE_SIZE", "64");
  cpp_macro_define("__GCC_HAVE_DWARF2_CFI_ASM", "1");
  cpp_macro_define("__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1", "1");
  cpp_macro_define("__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2", "1");
  cpp_macro_define("__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4", "1");
  cpp_macro_define("__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8", "1");
  cpp_macro_define("__GCC_IEC_559", "2");
  cpp_macro_define("__GCC_IEC_559_COMPLEX", "2");
  cpp_macro_define("__GNUC__", "13");
  cpp_macro_define("__GNUC_EXECUTION_CHARSET_NAME", "\"UTF-8\"");
  cpp_macro_define("__GNUC_MINOR__", "1");
  cpp_macro_define("__GNUC_PATCHLEVEL__", "0");
  cpp_macro_define("__GNUC_STDC_INLINE__", "1");
  cpp_macro_define("__GNUC_WIDE_EXECUTION_CHARSET_NAME", "\"UTF-32LE\"");
  cpp_macro_define("__gnu_linux__", "1");
  cpp_macro_define("__GXX_ABI_VERSION", "1018");
  cpp_macro_define("__HAVE_SPECULATION_SAFE_VALUE", "1");
  cpp_macro_define("__INT16_C(c)", "c");
  cpp_macro_define("__INT16_MAX__", "0x7fff");
  cpp_macro_define("__INT16_TYPE__", "short int");
  cpp_macro_define("__INT32_C(c)", "c");
  cpp_macro_define("__INT32_MAX__", "0x7fffffff");
  cpp_macro_define("__INT32_TYPE__", "int");
  cpp_macro_define("__INT64_C(c)", "c ## L");
  cpp_macro_define("__INT64_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INT64_TYPE__", "long int");
  cpp_macro_define("__INT8_C(c)", "c");
  cpp_macro_define("__INT8_MAX__", "0x7f");
  cpp_macro_define("__INT8_TYPE__", "signed char");
  cpp_macro_define("__INT_FAST16_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INT_FAST16_TYPE__", "long int");
  cpp_macro_define("__INT_FAST16_WIDTH__", "64");
  cpp_macro_define("__INT_FAST32_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INT_FAST32_TYPE__", "long int");
  cpp_macro_define("__INT_FAST32_WIDTH__", "64");
  cpp_macro_define("__INT_FAST64_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INT_FAST64_TYPE__", "long int");
  cpp_macro_define("__INT_FAST64_WIDTH__", "64");
  cpp_macro_define("__INT_FAST8_MAX__", "0x7f");
  cpp_macro_define("__INT_FAST8_TYPE__", "signed char");
  cpp_macro_define("__INT_FAST8_WIDTH__", "8");
  cpp_macro_define("__INT_LEAST16_MAX__", "0x7fff");
  cpp_macro_define("__INT_LEAST16_TYPE__", "short int");
  cpp_macro_define("__INT_LEAST16_WIDTH__", "16");
  cpp_macro_define("__INT_LEAST32_MAX__", "0x7fffffff");
  cpp_macro_define("__INT_LEAST32_TYPE__", "int");
  cpp_macro_define("__INT_LEAST32_WIDTH__", "32");
  cpp_macro_define("__INT_LEAST64_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INT_LEAST64_TYPE__", "long int");
  cpp_macro_define("__INT_LEAST64_WIDTH__", "64");
  cpp_macro_define("__INT_LEAST8_MAX__", "0x7f");
  cpp_macro_define("__INT_LEAST8_TYPE__", "signed char");
  cpp_macro_define("__INT_LEAST8_WIDTH__", "8");
  cpp_macro_define("__INT_MAX__", "0x7fffffff");
  cpp_macro_define("__INTMAX_C(c)", "c ## L");
  cpp_macro_define("__INTMAX_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INTMAX_TYPE__", "long int");
  cpp_macro_define("__INTMAX_WIDTH__", "64");
  cpp_macro_define("__INTPTR_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__INTPTR_TYPE__", "long int");
  cpp_macro_define("__INTPTR_WIDTH__", "64");
  cpp_macro_define("__INT_WIDTH__", "32");
  cpp_macro_define("__k8", "1");
  cpp_macro_define("__k8__", "1");
  cpp_macro_define("__LDBL_DECIMAL_DIG__", "21");
  cpp_macro_define("__LDBL_DENORM_MIN__", "3.64519953188247460252840593361941982e-4951L");
  cpp_macro_define("__LDBL_DIG__", "18");
  cpp_macro_define("__LDBL_EPSILON__", "1.08420217248550443400745280086994171e-19L");
  cpp_macro_define("__LDBL_HAS_DENORM__", "1");
  cpp_macro_define("__LDBL_HAS_INFINITY__", "1");
  cpp_macro_define("__LDBL_HAS_QUIET_NAN__", "1");
  cpp_macro_define("__LDBL_IS_IEC_60559__", "1");
  cpp_macro_define("__LDBL_MANT_DIG__", "64");
  cpp_macro_define("__LDBL_MAX_10_EXP__", "4932");
  cpp_macro_define("__LDBL_MAX__", "1.18973149535723176502126385303097021e+4932L");
  cpp_macro_define("__LDBL_MAX_EXP__", "16384");
  cpp_macro_define("__LDBL_MIN_10_EXP__", "(-4931)");
  cpp_macro_define("__LDBL_MIN__", "3.36210314311209350626267781732175260e-4932L");
  cpp_macro_define("__LDBL_MIN_EXP__", "(-16381)");
  cpp_macro_define("__LDBL_NORM_MAX__", "1.18973149535723176502126385303097021e+4932L");
  cpp_macro_define("__linux", "1");
  cpp_macro_define("__linux__", "1");
  cpp_macro_define("linux", "1");
  cpp_macro_define("__LONG_LONG_MAX__", "0x7fffffffffffffffLL");
  cpp_macro_define("__LONG_LONG_WIDTH__", "64");
  cpp_macro_define("__LONG_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__LONG_WIDTH__", "64");
  cpp_macro_define("__LP64__", "1");
  cpp_macro_define("_LP64", "1");
  cpp_macro_define("__MMX__", "1");
  cpp_macro_define("__MMX_WITH_SSE__", "1");
  cpp_macro_define("__NO_INLINE__", "1");
  cpp_macro_define("__ORDER_BIG_ENDIAN__", "4321");
  cpp_macro_define("__ORDER_LITTLE_ENDIAN__", "1234");
  cpp_macro_define("__ORDER_PDP_ENDIAN__", "3412");
  cpp_macro_define("__pic__", "2");
  cpp_macro_define("__PIC__", "2");
  cpp_macro_define("__pie__", "2");
  cpp_macro_define("__PIE__", "2");
  cpp_macro_define("__PRAGMA_REDEFINE_EXTNAME", "1");
  cpp_macro_define("__PTRDIFF_MAX__", "0x7fffffffffffffffL");
  cpp_macro_define("__PTRDIFF_TYPE__", "long int");
  cpp_macro_define("__PTRDIFF_WIDTH__", "64");
  cpp_macro_define("__REGISTER_PREFIX__", "");
  cpp_macro_define("__SCHAR_MAX__", "0x7f");
  cpp_macro_define("__SCHAR_WIDTH__", "8");
  cpp_macro_define("__SEG_FS", "1");
  cpp_macro_define("__SEG_GS", "1");
  cpp_macro_define("__SHRT_MAX__", "0x7fff");
  cpp_macro_define("__SHRT_WIDTH__", "16");
  cpp_macro_define("__SIG_ATOMIC_MAX__", "0x7fffffff");
  cpp_macro_define("__SIG_ATOMIC_MIN__", "(-__SIG_ATOMIC_MAX__ - 1)");
  cpp_macro_define("__SIG_ATOMIC_TYPE__", "int");
  cpp_macro_define("__SIG_ATOMIC_WIDTH__", "32");
  cpp_macro_define("__SIZE_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__SIZEOF_DOUBLE__", "8");
  cpp_macro_define("__SIZEOF_FLOAT128__", "16");
  cpp_macro_define("__SIZEOF_FLOAT__", "4");
  cpp_macro_define("__SIZEOF_FLOAT80__", "16");
  cpp_macro_define("__SIZEOF_INT128__", "16");
  cpp_macro_define("__SIZEOF_INT__", "4");
  cpp_macro_define("__SIZEOF_LONG__", "8");
  cpp_macro_define("__SIZEOF_LONG_DOUBLE__", "16");
  cpp_macro_define("__SIZEOF_LONG_LONG__", "8");
  cpp_macro_define("__SIZEOF_POINTER__", "8");
  cpp_macro_define("__SIZEOF_PTRDIFF_T__", "8");
  cpp_macro_define("__SIZEOF_SHORT__", "2");
  cpp_macro_define("__SIZEOF_SIZE_T__", "8");
  cpp_macro_define("__SIZEOF_WCHAR_T__", "4");
  cpp_macro_define("__SIZEOF_WINT_T__", "4");
  cpp_macro_define("__SIZE_TYPE__", "long unsigned int");
  cpp_macro_define("__SIZE_WIDTH__", "64");
  cpp_macro_define("__SSE__", "1");
  cpp_macro_define("__SSE2__", "1");
  cpp_macro_define("__SSE2_MATH__", "1");
  cpp_macro_define("__SSE_MATH__", "1");
  cpp_macro_define("__SSP_STRONG__", "3");
  cpp_macro_define("__STDC__", "1");
  cpp_macro_define("__STDC_HOSTED__", "1");
  cpp_macro_define("__STDC_IEC_559__", "1");
  cpp_macro_define("__STDC_IEC_559_COMPLEX__", "1");
  cpp_macro_define("__STDC_ISO_10646__", "201706L");
  cpp_macro_define("_STDC_PREDEF_H", "1");
  cpp_macro_define("__STDC_UTF_16__", "1");
  cpp_macro_define("__STDC_UTF_32__", "1");
  cpp_macro_define("__STDC_VERSION__", "201710L");
  cpp_macro_define("__UINT16_C(c)", "c");
  cpp_macro_define("__UINT16_MAX__", "0xffff");
  cpp_macro_define("__UINT16_TYPE__", "short unsigned int");
  cpp_macro_define("__UINT32_C(c)", "c ## U");
  cpp_macro_define("__UINT32_MAX__", "0xffffffffU");
  cpp_macro_define("__UINT32_TYPE__", "unsigned int");
  cpp_macro_define("__UINT64_C(c)", "c ## UL");
  cpp_macro_define("__UINT64_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINT64_TYPE__", "long unsigned int");
  cpp_macro_define("__UINT8_C(c)", "c");
  cpp_macro_define("__UINT8_MAX__", "0xff");
  cpp_macro_define("__UINT8_TYPE__", "unsigned char");
  cpp_macro_define("__UINT_FAST16_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINT_FAST16_TYPE__", "long unsigned int");
  cpp_macro_define("__UINT_FAST32_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINT_FAST32_TYPE__", "long unsigned int");
  cpp_macro_define("__UINT_FAST64_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINT_FAST64_TYPE__", "long unsigned int");
  cpp_macro_define("__UINT_FAST8_MAX__", "0xff");
  cpp_macro_define("__UINT_FAST8_TYPE__", "unsigned char");
  cpp_macro_define("__UINT_LEAST16_MAX__", "0xffff");
  cpp_macro_define("__UINT_LEAST16_TYPE__", "short unsigned int");
  cpp_macro_define("__UINT_LEAST32_MAX__", "0xffffffffU");
  cpp_macro_define("__UINT_LEAST32_TYPE__", "unsigned int");
  cpp_macro_define("__UINT_LEAST64_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINT_LEAST64_TYPE__", "long unsigned int");
  cpp_macro_define("__UINT_LEAST8_MAX__", "0xff");
  cpp_macro_define("__UINT_LEAST8_TYPE__", "unsigned char");
  cpp_macro_define("__UINTMAX_C(c)", "c ## UL");
  cpp_macro_define("__UINTMAX_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINTMAX_TYPE__", "long unsigned int");
  cpp_macro_define("__UINTPTR_MAX__", "0xffffffffffffffffUL");
  cpp_macro_define("__UINTPTR_TYPE__", "long unsigned int");
  cpp_macro_define("__unix", "1");
  cpp_macro_define("__unix__", "1");
  cpp_macro_define("unix", "1");
  cpp_macro_define("__USER_LABEL_PREFIX__", "");
  cpp_macro_define("__VERSION__", "\"13.1.0\"");
  cpp_macro_define("__WCHAR_MAX__", "0x7fffffff");
  cpp_macro_define("__WCHAR_MIN__", "(-__WCHAR_MAX__ - 1)");
  cpp_macro_define("__WCHAR_TYPE__", "int");
  cpp_macro_define("__WCHAR_WIDTH__", "32");
  cpp_macro_define("__WINT_MAX__", "0xffffffffU");
  cpp_macro_define("__WINT_MIN__", "0U");
  cpp_macro_define("__WINT_TYPE__", "unsigned int");
  cpp_macro_define("__WINT_WIDTH__", "32");
  cpp_macro_define("__x86_64", "1");
  cpp_macro_define("__x86_64__", "1");
}
/* __DATE__ is expanded to the current date, e.g. "May 17 2020". */
static char*
format_date(struct tm* tm) {
  static char mon[][4] = {
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec",
  };

  return cpp_format("\"%s %2d %d\"", mon[tm->tm_mon], tm->tm_mday, tm->tm_year + 1900);
}

/* __TIME__ is expanded to the current time, e.g. "13:34:03". */
static char*
format_time(struct tm* tm) {
  return cpp_format("\"%02d:%02d:%02d\"", tm->tm_hour, tm->tm_min, tm->tm_sec);
}
