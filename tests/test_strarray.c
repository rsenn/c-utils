#include "unit_test.h"
#include "../lib/strarray.h"
#include "../lib/stralloc.h"

/*
 * int strarray_from_argv(int argc, const char* const argv[], strarray* arr);
 */
TEST(test_strarray_from_argv) {
  const char* const headers[] = {"lib/array.h", "lib/buffer.h", "lib/xml.h"};
  strarray arr;

  strarray_init(&arr);
  strarray_from_argv(3, headers, &arr);

  ASSERT_EQ(3, strarray_size(&arr));
  ASSERT_STR_EQUAL("lib/array.h", strarray_at(&arr, 0));
  ASSERT_STR_EQUAL("lib/xml.h", strarray_at(&arr, 2));

  strarray_free(&arr);
}

/*
 * int strarray_removesuffixs(strarray* arr, const char* s);
 * int strarray_joins(const strarray* arr, stralloc* sa, const char* sep);
 */
TEST(test_strarray_removesuffixs_joins) {
  const char* const headers[] = {"lib/array.h", "lib/buffer.h", "lib/xml.h"};
  strarray arr;
  stralloc sa;

  strarray_init(&arr);
  stralloc_init(&sa);
  strarray_from_argv(3, headers, &arr);

  strarray_removesuffixs(&arr, ".h");
  strarray_joins(&arr, &sa, ", ");
  stralloc_nul(&sa);

  ASSERT_STR_EQUAL("lib/array, lib/buffer, lib/xml", sa.s);

  stralloc_free(&sa);
  strarray_free(&arr);
}

#define RUN_STRARRAY_TESTS() \
  RUN(test_strarray_from_argv); \
  RUN(test_strarray_removesuffixs_joins)

TESTS(strarray) { RUN_STRARRAY_TESTS(); }
