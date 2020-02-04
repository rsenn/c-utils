#include "lib/buffer.h"
#include "lib/errmsg.h"
#include "lib/uint32.h"

uint32
crc32(uint32 crc, const char* data, size_t size) {
  uint32 i, r = ~0;
  const char* end = data + size;

  while(data < end) {
    r ^= *data++;

    for(i = 0; i < 8; i++) {
      uint32 t = ~((r & 1) - 1);
      r = (r >> 1) ^ (0xedb88320 & t);
    }
  }

  return ~r;
}

#if 1
int
main(int argc, char* argv[]) {
  int i;

  errmsg_iam(argv[0]);

  for(i = 1; i < argc; ++i) {
    buffer b;
    unsigned long crc = 0;

    if(buffer_mmapread(&b, argv[i]) == -1) {
      errmsg_warn("error mapping '", argv[i], "': ", 0);
      b.x = 0;
      b.n = 0;
    //  return 127;
    }

    crc = crc32(crc, (const char*)b.x, b.n);

    buffer_putxlong0(buffer_1, crc, 8);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, argv[i]);
    buffer_putnlflush(buffer_1);

    buffer_close(&b);
  }

  return 0;
}
#endif
