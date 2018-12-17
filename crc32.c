#include "lib/buffer.h"
#include "lib/errmsg.h"
#include <zlib.h>

int
main(int argc, char* argv[]) {
  int i;

  errmsg_iam(argv[0]);

  for(i = 1; i < argc; ++i) {
    buffer b;
    unsigned long crc = 0;

    if(buffer_mmapread(&b, argv[i]) == -1) {
    	errmsg_warn("error mapping '", argv[i], "': ", 0);
    	return 127;
    }

    crc = crc32(crc, (const Bytef*)b.x, b.n);

    buffer_putxlong0(buffer_1, crc, 8);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, argv[i]);
    buffer_putnlflush(buffer_1);   

    buffer_close(&b);
  }

  return 0;
}
