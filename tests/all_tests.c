#include "unit_test.h"
#include "../lib/errmsg.h"
#include <stdlib.h>
#include <string.h>

int
main(int argc, char* argv[]) {
  errmsg_iam(argv[0]);

  /* tests/test_wait.c spawns a copy of this executable via process_create()
   * to get a child process with a known exit code -- portable to
   * WINDOWS_NATIVE, which has no fork(). */
  if(argc == 3 && !strcmp(argv[1], "--wait-child"))
    exit(atoi(argv[2]));

  return unit_test_main(argc, argv);
}