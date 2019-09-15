#include "lib/ihex.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"

int main() {

  static buffer input;
  const char* filename = "/home/roman/Sources/lc-meter/build/xc8/Debug/LC-meter.hex";
  static stralloc sa;

  buffer_mmapread(&input, filename);

  buffer_getline_sa(&input, &sa);

  ihex_record rec;

  ssize_t ret = ihex_read_record(&rec, sa.s, sa.len);

  buffer_puts(buffer_2, "ret = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);


}
