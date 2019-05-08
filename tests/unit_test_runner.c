#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/path.h"
#include "../lib/stralloc.h"
#include "../lib/strlist.h"
#include "../lib/tuple.h"
/*
#include "test_path.c"
#include "test_strlist.c"
*/
TESTS(buffer);
TESTS(stralloc);
TESTS(strlist);
TESTS(path);
TESTS(tuple);

START() {
  EXEC(buffer);
  EXEC(stralloc);
  EXEC(strlist);
  EXEC(path);
  EXEC(tuple);
}
