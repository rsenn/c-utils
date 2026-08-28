#include "unit_test.h"
#include "../lib/buffer.h"
#include "../lib/path.h"
#include "../lib/strlist.h"
#include "../lib/iarray.h"
// #include "../lib/tuple.h"
/*
#include "test_path.c"
#include "test_strlist.c"
*/
TESTS(stralloc);
TESTS(strlist);
TESTS(path);
TESTS(tuple);
TESTS(hashmap);
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
TESTS(str);
TESTS(scan);
TESTS(unix);
TESTS(socket);
TESTS(mmap);
TESTS(env);
TESTS(dir);
TESTS(safemult);
TESTS(open);
TESTS(taia);
TESTS(wait);
TESTS(uint32);
TESTS(uint64);
TESTS(uint16);
TESTS(seek);
TESTS(tai);
TESTS(ndelay);

START() {
  EXEC(stralloc);
  EXEC(strlist);
  EXEC(path);
  EXEC(tuple);
  EXEC(hashmap);
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
  EXEC(str);
  EXEC(scan);
  EXEC(unix);
  EXEC(socket);
  EXEC(mmap);
  EXEC(env);
  EXEC(dir);
  EXEC(safemult);
  EXEC(open);
  EXEC(taia);
  EXEC(wait);
  EXEC(uint32);
  EXEC(uint64);
  EXEC(uint16);
  EXEC(seek);
  EXEC(tai);
  EXEC(ndelay);
}
