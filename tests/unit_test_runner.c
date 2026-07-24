#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/path.h"
#include "../lib/strlist.h"
#include "../lib/iarray.h"
//#include "../lib/tuple.h"
/*
#include "test_path.c"
#include "test_strlist.c"
*/
TESTS(stralloc);
TESTS(strlist);
TESTS(path);
TESTS(tuple);
TESTS(iarray);
TESTS(alloc);
TESTS(array);
TESTS(buffer);
TESTS(byte);
TESTS(case);
TESTS(errmsg);
TESTS(fmt);
TESTS(iob);
TESTS(io);
TESTS(iom);

START() {
  EXEC(stralloc);
  EXEC(strlist);
  EXEC(path);
  EXEC(tuple);
  EXEC(iarray);
  EXEC(alloc);
  EXEC(array);
  EXEC(buffer);
  EXEC(byte);
  EXEC(case);
  EXEC(errmsg);
  EXEC(fmt);
  EXEC(iob);
  EXEC(io);
  EXEC(iom);
}
