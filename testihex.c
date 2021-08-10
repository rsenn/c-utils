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
  size_t i;
  for(i = 0; i < n; i++) {
    if(i)
      buffer_putspace(buffer_1);

    buffer_putxlong0(buffer_1, x[i], 2);
  }
}

uint32
mem_top(ihex_file* h) {
  ihex_record* r;
  uint32 top = 0;
  slink_foreach(h->records, r) {
    if(top < r->offset + r->length)
      top = r->offset + r->length;
  }
  return top;
}

uint32
mem_bottom(ihex_file* h) {
  ihex_record* r;
  uint32 bottom = 0xffffffff;
  slink_foreach(h->records, r) {
    if(bottom > r->offset)
      bottom = r->offset;
  }
  return bottom;
}

int
main(int argc, char* argv[]) {
  ssize_t ret;
  static buffer input;
  const char* filename = argv[1] ? argv[1]
                                 : "/home/roman/Dokumente/Sources/xc8/pictest/bootloaders/18f2550-usb-hid-xc8/FIRMWARE/"
                                   "PIC18F2550/18F2550-MPLAB.X/dist/default/production/18F2550-MPLAB.X.production.hex";
  static stralloc sa;
  size_t sz = 0;
  const char* x = mmap_read(filename, &sz);

  buffer_mmapread(&input, filename);
  buffer_getline_sa(&input, &sa);

  {
    ihex_record* recp;
    ihex_file ihx;

    ret = ihex_load_record(&recp, sa.s, sa.len);
    ret = ihex_load_buf(&ihx, x, sz);

    uint32 top = mem_top(&ihx);
    uint32 bottom = mem_bottom(&ihx);
    buffer_puts(buffer_1, "mem top = 0x");
    buffer_putxlong0(buffer_1, top, 4);
    buffer_putnlflush(buffer_1);
    buffer_puts(buffer_1, "mem bottom = 0x");
    buffer_putxlong0(buffer_1, bottom, 4);
    buffer_putnlflush(buffer_1);

    slink_foreach(ihx.records, recp) {

      buffer_puts(buffer_1, "record addr = 0x");
      buffer_putxlong0(buffer_1, recp->offset, 4);
      buffer_puts(buffer_1, ", len = ");
      buffer_putulong(buffer_1, recp->length);
      buffer_puts(buffer_1, ", type = ");
      buffer_putulong(buffer_1, recp->type);
      buffer_puts(buffer_1, ", data = ");
      putdata(recp->data, recp->length);
      buffer_putnlflush(buffer_1);
    }
  }

  buffer_puts(buffer_1, "ret = ");
  buffer_putlong(buffer_1, ret);
  buffer_putnlflush(buffer_1);
}
