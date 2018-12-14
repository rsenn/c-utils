#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/open.h"
#include "lib/mmap.h"
#include "lib/byte.h"
#include "lib/str.h"

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
  buffer deflate, gzout, inflate, gzin;
  stralloc fname;

  const char* filename =  argv[1] ? argv[1] : "/home/roman/Sources/file-5.34.tar.xz";

  if(buffer_mmapprivate(&input, filename) < 0) {
    buffer_putm_2(buffer_2, "ERROR opening: ", filename);
    buffer_putnlflush(buffer_2);
    return 1;
  }

  stralloc_init(&fname);
  mmap_filename(input.x, &fname);

  buffer_puts(buffer_2, "Filename: ");
  buffer_putsa(buffer_2, &fname);
  buffer_putnlflush(buffer_2);

  buffer_truncfile(&output, "output.lzma");

#if defined(HAVE_LIBBZ2)
  buffer_bz2(&decompress, &input, 0);
  buffer_bz2(&compress, &output, 1);

  buffer_puts(&compress, "bz2 compressed test text\n");
  buffer_flush(&compress);
  buffer_close(&compress);
#elif defined(HAVE_LIBLZMA)
  buffer_lzma(&decompress, &input, 0);
  buffer_lzma(&compress, &output, 1);

  buffer_puts(&compress, "lzma compressed test text\n");
  buffer_flush(&compress);
  buffer_close(&compress);
#endif

  //buffer_deflate(&compress, &output, 3);

  buffer_copy(buffer_1, &decompress);
//

  buffer_truncfile(&gzout, "output.gz");

#ifdef HAVE_LIBZ
  buffer_deflate(&deflate, &gzout, 9);

  buffer_puts(&deflate, "gzipp'd test text\n\nblah blah blah\n");
  buffer_flush(&deflate);
  buffer_close(&deflate);

  if(buffer_mmapprivate(&gzin, "output.gz") < 0) {
    buffer_putsflush(buffer_2, "ERROR\n");
    return 1;
  }

  buffer_inflate(&inflate, &gzin);

  buffer_copy(buffer_1, &inflate);
#endif
  buffer_flush(buffer_1);

  return 0;
}
