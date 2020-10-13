#include "../io.h"

void
io_onlywantread(fd_t d) {
  io_dontwantwrite(d);
  io_wantread(d);
}