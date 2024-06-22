#include "../io.h"
#include "../buffer.h"

void
io_onlywantwrite(fd_type d) {
#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_onlywantwrite", 30);
  buffer_puts(buffer_2, "d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif
  io_dontwantread(d);
  io_wantwrite(d);
}