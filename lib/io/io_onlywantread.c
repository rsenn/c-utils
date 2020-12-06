#include "../io.h"
#include "../buffer.h"

void
io_onlywantread(fd_t d) {
#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_onlywantread", 30);
  buffer_puts(buffer_2, "d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif
  io_dontwantwrite(d);
  io_wantread(d);
}