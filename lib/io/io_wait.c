#include "../io.h"
#include "../io_internal.h"

int64
io_wait() {
  return io_waituntil2(-1);
}
