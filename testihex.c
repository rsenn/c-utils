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
  ihex_addr a = {0};
  slink_foreach(h->records, r) {
    if(r->type == 4) {
      uint16_unpack_big(r->data, &a.hi16);
      continue;
    }
    a.lo16 = r->offset;
    if(top < a.off32 + r->length)
      top = a.off32 + r->length;
  }
  return top;
}

uint32
mem_bottom(ihex_file* h) {
  ihex_record* r;
  uint32 bottom = 0xffffffff;
  ihex_addr a = {0};

  slink_foreach(h->records, r) {
    if(r->type == 4) {
      uint16_unpack_big(r->data, &a.hi16);
      continue;
    }
    a.lo16 = r->offset;

    if(bottom > a.off32)
      bottom = a.off32;
  }
  return bottom;
}

int
main(int argc, char* argv[]) {
  ssize_t ret;
  static buffer input, output;
  const char* filename = argv[1] ? argv[1]
                                 : "/home/roman/Dokumente/Sources/xc8/pictest/bootloaders/18f2550-usb-hid-xc8/FIRMWARE/"
                                   "PIC18F2550/18F2550-MPLAB.X/dist/default/production/18F2550-MPLAB.X.production.hex";
  static stralloc sa;
  size_t sz = 0;
  const char* x = mmap_read(filename, &sz);

  buffer_mmapread(&input, filename);
  buffer_getline_sa(&input, &sa);

  {
    ihex_record* r;
    ihex_file ihx;
    ihex_addr a = {0}, prev = {0};

    ret = ihex_record_read(&r, sa.s, sa.len);
    ret = ihex_read_buf(&ihx, x, sz);

    uint32 top = mem_top(&ihx);
    uint32 bottom = mem_bottom(&ihx);
    buffer_puts(buffer_1, "mem top = 0x");
    buffer_putxlong0(buffer_1, top, 4);
    buffer_putnlflush(buffer_1);
    buffer_puts(buffer_1, "mem bottom = 0x");
    buffer_putxlong0(buffer_1, bottom, 4);
    buffer_putnlflush(buffer_1);

    slink_foreach(ihx.records, r) {
      if(r->type == 4) {
        uint16_unpack_big(r->data, &a.hi16);
        continue;
      }

      a.lo16 = r->offset;

      if(prev.off32 < a.off32) {
        buffer_puts(buffer_1, "empty space = 0x");
        buffer_putxlong0(buffer_1, a.off32 - prev.off32, 6);
        buffer_puts(buffer_1, " bytes");
        buffer_putnlflush(buffer_1);
      }

      buffer_puts(buffer_1, "record addr = 0x");
      buffer_putxlong0(buffer_1, a.off32, 6);
      buffer_puts(buffer_1, ", len = ");
      buffer_putulong0(buffer_1, r->length, 3);
      buffer_puts(buffer_1, ", type = ");
      buffer_putulong(buffer_1, r->type);
      buffer_puts(buffer_1, ", data = ");
      putdata(r->data, r->length);
      buffer_putnlflush(buffer_1);

      prev = a;
      prev.off32 += r->length;
    }

    buffer_writefile(&output, "testihex.hex");
    ihex_write(&ihx, &output);
    buffer_close(&output);
  }

  buffer_puts(buffer_1, "ret = ");
  buffer_putlong(buffer_1, ret);
  buffer_putnlflush(buffer_1);
}
