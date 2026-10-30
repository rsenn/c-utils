#include "unit_test.h"
#include "../lib/ini.h"
#include "../lib/buffer.h"
#include "../lib/open.h"
#include <unistd.h>

static const char text[] = "[Main]\r\nname=value\r\ncount=42\r\n[Other]\r\nkey=x\r\n";

/*
 * void ini_read(buffer*, ini_section_t**);
 */
TEST(test_ini_read) {
  buffer b;
  ini_section_t* ini;

  buffer_frombuf(&b, text, sizeof(text) - 1);
  ini_read(&b, &ini);

  ASSERT_NE(NULL, ini);
  ASSERT_SA_EQUALS(&ini->name, "Main");
  ASSERT_STR_EQUAL("value", ini_get(ini, "name"));
  ASSERT_STR_EQUAL("42", ini_get(ini, "count"));
  ASSERT_NE(NULL, ini->next);
  ASSERT_SA_EQUALS(&ini->next->name, "Other");
  ASSERT_STR_EQUAL("x", ini_get(ini->next, "key"));
  ASSERT_EQ(NULL, ini_get(ini, "missing"));
}

/*
 * ini_section_t* ini_section(ini_section_t*, const char* name);
 */
TEST(test_ini_section) {
  buffer b;
  ini_section_t* ini;

  buffer_frombuf(&b, text, sizeof(text) - 1);
  ini_read(&b, &ini);

  ASSERT_EQ(ini->next, ini_section(ini, "other"));
  ASSERT_EQ(ini, ini_section(ini, "MAIN"));
  ASSERT_EQ(NULL, ini_section(ini, "nonexistent"));
}

/*
 * ini_new, ini_set, ini_set_long, ini_get
 */
TEST(test_ini_set) {
  ini_section_t* ini = NULL;
  ini_section_t* s = ini_new(&ini, "Section");

  ASSERT_EQ(s, ini);

  ini_set(s, "a", "b");
  ini_set_long(s, "n", -17);

  ASSERT_STR_EQUAL("b", ini_get(s, "a"));
  ASSERT_STR_EQUAL("-17", ini_get(s, "n"));
}

/*
 * void ini_write(buffer*, ini_section_t*, int utf16);
 */
TEST(test_ini_write) {
  buffer in, out, again;
  ini_section_t *ini, *ini2;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  ini_read(&in, &ini);

  buffer_write_fd(&out, open_trunc("test_ini.tmp"));
  ini_write(&out, ini, 0);
  buffer_close(&out);

  /* round trip: what was written reads back the same */
  ASSERT_EQ(0, buffer_mmapread(&again, "test_ini.tmp"));
  ini_read(&again, &ini2);
  buffer_close(&again);
  unlink("test_ini.tmp");

  ASSERT_NE(NULL, ini2);
  ASSERT_STR_EQUAL("value", ini_get(ini2, "name"));
  ASSERT_STR_EQUAL("x", ini_get(ini2->next, "key"));
}

#define RUN_INI_TESTS() \
  RUN(test_ini_read); \
  RUN(test_ini_section); \
  RUN(test_ini_set); \
  RUN(test_ini_write)

TESTS(ini) { RUN_INI_TESTS(); }
