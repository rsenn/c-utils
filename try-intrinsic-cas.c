#include <stdlib.h>

int
main() {
  long v;
  __sync_val_compare_and_swap(&v, 1, 0);
  return 0;
}