#include "../io.h"

void
io_onlywantwrite(fd_t d) {
  io_dontwantread(d);
  io_wantwrite(d);
}