#include "lib/buffer.h"
#include "lib/open.h"
#include "lib/unix.h"
#include "lib/str.h"
#include "lib/unix.h"
#include "lib/mmap.h"
#include "lib/byte.h"
#include "lib/iarray.h"
#include "lib/io.h"
#include "lib/getopt.h"
#include "lib/path.h"
#include "lib/case.h"

#include <stdlib.h>
#include "lib/bool.h"

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

typedef enum compression_type { C_UNKNOWN = 0, C_GZ, C_BZ2, C_LZMA, C_XZ, C_BROTLI } compression_type;

compression_type
compression_from_ext(const char* ext) {
  if(case_equals(ext, "gz") || case_equals(ext, "tgz"))
    return C_GZ;

  if(case_equals(ext, "bz2") || case_equals(ext, "tbz2") || case_equals(ext, "tbz"))
    return C_BZ2;

  if(case_equals(ext, "xz") || case_equals(ext, "txz"))
    return C_XZ;

  if(case_equals(ext, "lzma"))
    return C_LZMA;

  if(case_equals(ext, "br"))
    return C_BROTLI;

  return C_UNKNOWN;
}

compression_type
compression_from_filename(const char* fn) {
  const char* ext = fn + str_rchr(fn, '.');
  if(*ext)
    return compression_from_ext(++ext);
  return C_UNKNOWN;
}

void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       argv0,
                       " [-o output] [infile or "
                       "stdin]\n\n",
                       "  -1 ... -11           "
                       "compression level; default is "
                       "3\n",
                       "\n",
                       "Supported types are:",
                       0);
#if HAVE_ZLIB
  buffer_puts(buffer_1, " gz");
#endif
#if HAVE_LIBBZ2
  buffer_puts(buffer_1, " bz2");
#endif
#if HAVE_LIBLZMA
  buffer_puts(buffer_1, " lzma xz");
#endif
#if HAVE_BROTLI
  buffer_puts(buffer_1, " br");
#endif
  buffer_putnlflush(buffer_1);
  exit(0);
}
int
main(int argc, char* argv[]) {
  int opt, index = 0;
  int level = 10;
  int decompress = -1;
  bool force = false;
  const char *in_filename = "-", *out_filename = "-";
  compression_type type = C_UNKNOWN, in_type = C_UNKNOWN, out_type = C_UNKNOWN;
  buffer infile, outfile;
  buffer *input, *output;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"10", 0, NULL, 10},
                           {"11", 0, NULL, 11},
                           {"compress", 0, NULL, 'c'},
                           {"decompress", 0, NULL, 'd'},
                           {"type", 1, NULL, 't'},
                           {"output", 1, NULL, 'o'},
                           {"force", 0, NULL, 'f'},

                           {0, 0, 0, 0}};

  while((opt = unix_getopt_long(argc, argv, "123456789cdt:o:hf", opts, &index)) != -1) {
    switch(opt) {
        // case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': level = opt - '0'; break;
      case 10: level = opt; break;
      case 11: level = opt; break;
      case 'f': force = true; break;
      case 'c': decompress = 0; break;
      case 'd': decompress = 1; break;
      case 't': type = compression_from_ext(unix_optarg); break;
      case 'o': out_filename = unix_optarg; break;
      case 'h': usage(str_basename(argv[0])); exit(EXIT_SUCCESS);
      default: /* '?' */
        buffer_putm_internal(buffer_2,
                             "Usage: ",
                             argv[0],
                             "[-t TYPE] [-o OUTPUT] "
                             "[file]\n",
                             0);
        exit(EXIT_FAILURE);
    }
  }

  if(argv[unix_optind])
    in_filename = argv[unix_optind];

  if(str_equal(in_filename, "-")) {
    input = buffer_0;
  } else {
    if(buffer_mmapprivate(&infile, in_filename) < 0) {
      buffer_putm_internal(buffer_2, "ERROR opening: ", in_filename, 0);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    input = &infile;
    if(in_type == C_UNKNOWN) {
      in_type = compression_from_filename(in_filename);
      if(in_type != C_UNKNOWN && decompress == -1)
        decompress = 1;
    }
  }

  if(str_equal(out_filename, "-")) {
    output = buffer_1;
  } else {
    if(!force && path_exists(out_filename)) {
      buffer_putm_internal(buffer_2,
                           "ERROR already exists (use "
                           "-f): ",
                           out_filename,
                           0);
      buffer_putnlflush(buffer_2);
      return 1;
    }

    if(buffer_truncfile(&outfile, out_filename) < 0) {
      buffer_putm_internal(buffer_2, "ERROR opening: ", out_filename, 0);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    output = &outfile;
    if(out_type == C_UNKNOWN)
      out_type = compression_from_filename(out_filename);
    if(out_type != C_UNKNOWN && decompress == -1)
      decompress = 0;
  }

  if(type != C_UNKNOWN) {
    if(in_type == C_UNKNOWN && decompress == 1)
      in_type = type;
    if(out_type == C_UNKNOWN && decompress == 0)
      out_type = type;
  }

  {
    compression_type type = decompress ? in_type : out_type;
    buffer cbuf;

    switch(type) {
      case C_GZ:
        if(decompress)
          buffer_inflate(&cbuf, input);
        else
          buffer_deflate(&cbuf, output, level);
        break;
      case C_BZ2: buffer_bz2(&cbuf, decompress ? input : output, decompress ? 0 : level); break;
      case C_XZ:
      case C_LZMA: buffer_lzma(&cbuf, decompress ? input : output, decompress ? 0 : level); break;
      case C_BROTLI: buffer_brotli(&cbuf, decompress ? input : output, decompress ? 0 : level); break;
      default:
        buffer_putm_internal(buffer_2,
                             "ERROR: Unable to detect "
                             "compression type from ",
                             in_filename,
                             0);
        buffer_putnlflush(buffer_2);
        exit(EXIT_FAILURE);
    }

    if(decompress == 0 && output == buffer_1) {
      buffer_putsflush(buffer_2,
                       "ERROR: Won't write "
                       "compressed data to a "
                       "terminal\n");
      exit(EXIT_FAILURE);
    }

    if(decompress) {
      buffer_copy(output, &cbuf);
    } else {
      buffer_copy(&cbuf, input);
      buffer_flush(&cbuf);
    }
    buffer_close(&cbuf);
    buffer_flush(output);
  }

  return 0;
}
