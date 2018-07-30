#include "lib/buffer.h"

int
buffer_copy(buffer* out, buffer* in) {
  ssize_t r, n = 0;
  char tmp[1024];

  while((r = buffer_get(in, tmp, sizeof(tmp))) > 0) {
     buffer_put(out, tmp, r);

     n += r;

  }
  buffer_flush(out);
  return n;
}

int
main(int argc, char* argv[])  {
  buffer input,  output, compress, decompress;
  buffer_mmapprivate(&input, argv[1] ? argv[1] : "/mnt/Newx20Data/Sources/gettext-0.19.8.1.tar.xz");

  buffer_truncfile(&output, "output.lzma");

  buffer_lzma(&decompress, &input, 0);
  buffer_lzma(&compress, &output, 1);

  buffer_copy(&compress, &decompress);

  buffer_flush(&compress);
  buffer_close(&compress);

  return 0;
}
