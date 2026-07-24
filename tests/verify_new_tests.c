#include "unit_test.h"
#include "../lib/buffer.h"
#include <stdio.h>

TESTS(alloc);
TESTS(array);
TESTS(buffer);
TESTS(byte);
TESTS(case);
TESTS(errmsg);

void
unit_test_execute(struct unit_test* mu_) {
  (void)mu_;
}

int
main(void) {
  static struct unit_test mu_i;
  struct unit_test* mu_ = &mu_i;

  mu_i.testlog = buffer_1;
  mu_i.faillog = buffer_2;

  unit_test_execute_alloc(mu_);
  unit_test_execute_array(mu_);
  unit_test_execute_buffer(mu_);
  unit_test_execute_byte(mu_);
  unit_test_execute_case(mu_);
  unit_test_execute_errmsg(mu_);

  buffer_flush(buffer_2);
  fprintf(stdout, "success=%d failure=%d\n", mu_->success, mu_->failure);
  return mu_->failure != 0;
}
