#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/strlist.h"
#include "../lib/path.h"
/*
#include "test_strlist.c"
#include "test_path.c"
*/
TESTS(path);
TESTS(strlist);

START() {

  EXEC(path);
  EXEC(strlist);
}