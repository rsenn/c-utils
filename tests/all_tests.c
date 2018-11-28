#include "unit_test.h"
#include "../lib/errmsg.h"

int
main(int argc, char* argv[]) {
  errmsg_iam(argv[0]);
  return unit_test_main(argc, argv);
}