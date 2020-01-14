#include "lib/ihex.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/byte.h"
#include "lib/slist.h"
#include "lib/uint8.h"

void
putdata(const uint8* x, size_t n) {
  for(size_t i = 0; i < n; i++) {
    if(i)
      buffer_putspace(buffer_2);

    buffer_putxlong0(buffer_2, x[i], 2);
  }
}

int
main(int argc, char* argv[]) {

  static buffer input;
  const char* filename = argv[1] ? argv[1] : "/home/roman/Dokumente/Sources/xc8/pictest/bootloaders/18f2550-usb-hid-xc8/FIRMWARE/PIC18F2550/18F2550-MPLAB.X/dist/default/production/18F2550-MPLAB.X.production.hex";
  static stralloc sa;
  size_t sz = 0;
  const char* x = mmap_read(filename, &sz);

  buffer_mmapread(&input, filename);
  buffer_getline_sa(&input, &sa);

  ihex_record* recp;
  ihex_file ihx;

  ssize_t ret = ihex_read_record(&recp, sa.s, sa.len);
  ret = ihex_read_buf(&ihx, x, sz);

  slink_foreach(ihx.records, recp) {
    buffer_puts(buffer_2, "record addr = 0x");
    buffer_putxlong0(buffer_2, recp->offset, 4);
    buffer_puts(buffer_2, ", len = ");
    buffer_putulong(buffer_2, recp->reclen);
    buffer_puts(buffer_2, ", type = ");
    buffer_putulong(buffer_2, recp->rectyp);
    buffer_puts(buffer_2, ", data = ");
    putdata(recp->data, recp->reclen);
    buffer_putnlflush(buffer_2);
  }

  buffer_puts(buffer_2, "ret = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
}
