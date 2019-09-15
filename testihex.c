#include "lib/ihex.h"

int main() {

  size_t n; 
  const char* x = mmap_read("/home/roman/Sources/lc-meter/build/xc8/Debug/LC-meter.hex", &n);
  ihex_record rec;

  ssize_t ret = ihex_read_record(&rec, x, byte_chr(x, n, '\n'));


}
