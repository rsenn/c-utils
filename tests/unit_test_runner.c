#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/path.h"
#include "../lib/strlist.h"
/*
#include "test_path.c"
#include "test_strlist.c"
*/
TESTS(stralloc);
TESTS(strlist);
TESTS(path);

START() {
  EXEC(stralloc);
  EXEC(strlist);
  EXEC(path);
}
