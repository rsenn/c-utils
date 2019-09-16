#include "lib/ihex.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"

int main() {

  static buffer input;
  const char* filename = "/home/roman/Sources/lc-meter/build/xc8/Debug/LC-meter.hex";
  static stralloc sa;
  size_t sz = 0;
  const char* x = mmap_private(filename, &sz);

  buffer_mmapread(&input, filename);
  buffer_getline_sa(&input, &sa);

  ihex_record* recp;
  ihex_file ihx;

  ssize_t ret = ihex_read_data(&recp, sa.s, sa.len);
  ret = ihex_read_buf(&ihx, x, sz);

  buffer_puts(buffer_2, "ret = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);


}
