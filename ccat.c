#include "lib/buffer.h"
#include "lib/str.h"
#include "lib/getopt.h"

#include <stdlib.h>

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

void
usage(const char* argv0) {
  buffer_putm(buffer_1,
              "Usage: ",
              argv0,
              " [-o output] [infile or stdin]\n\n",
              "  -1 ... -9           compression level; default is 3\n",
              "\n",
              "Supported types are:");
#if HAVE_LIBZ
  buffer_puts(buffer_1, " gz");
#endif
#if HAVE_LIBBZ2
  buffer_puts(buffer_1, " bz2");
#endif
#if HAVE_LIBLZMA
  buffer_puts(buffer_1, " lzma xz");
#endif
  buffer_putnlflush(buffer_1);
  exit(0);
}

typedef enum compression_type {
  C_UNKNOWN = 0,
  C_GZ,
  C_BZ2,
  C_LZMA,
  C_XZ,
} compression_type;

compression_type
compression_from_ext(const char* ext) {
  if(str_case_equal(ext, "gz") || str_case_equal(ext, "tgz")) return C_GZ;

  if(str_case_equal(ext, "bz2") || str_case_equal(ext, "tbz2") || str_case_equal(ext, "tbz")) return C_BZ2;

  if(str_case_equal(ext, "xz") || str_case_equal(ext, "txz")) return C_XZ;

  if(str_case_equal(ext, "lzma")) return C_LZMA;

  return C_UNKNOWN;
}

compression_type
compression_from_filename(const char* fn) {
  const char* ext = fn + str_rchr(fn, '.');
  if(*ext) return compression_from_ext(++ext);
  return C_UNKNOWN;
}

int
main(int argc, char* argv[]) {
  int opt;
  int level = 3;
  int decompress = 0;
  const char* in_filename = "-";
  const char* out_filename = "-";
  compression_type in_type = C_UNKNOWN;
  compression_type out_type = C_UNKNOWN;
  buffer *input, *output;

  while((opt = getopt(argc, argv, "123456789dt:o:h")) != -1) {
    switch(opt) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        level = opt - '0';
        break;
      case 'd':
        decompress = 1;
        break;
      case 't':
        in_type = compression_from_ext(optarg);
        break;
      case 'o':
        out_filename = optarg;
        break;
      case 'h':
        usage(str_basename(argv[0]));
        exit(EXIT_SUCCESS);
      default: /* '?' */
        buffer_putm(buffer_2, "Usage: ", argv[0], "[-t TYPE] [-o OUTPUT] [file]\n");
        exit(EXIT_FAILURE);
    }
  }

  buffer infile, outfile;

  if(argv[optind]) in_filename = argv[optind];

  if(str_equal(in_filename, "-")) {
    input = buffer_0;
  } else {
    if(buffer_mmapprivate(&infile, in_filename) < 0) {
      buffer_putm(buffer_2, "ERROR opening: ", in_filename);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    input = &infile;
    if(in_type == C_UNKNOWN) {
      in_type = compression_from_filename(in_filename);
      if(in_type != C_UNKNOWN) decompress = 1;
    }
  }

  if(str_equal(out_filename, "-")) {
    output = buffer_1;
  } else {
    if(buffer_truncfile(&outfile, out_filename) < 0) {
      buffer_putm(buffer_2, "ERROR opening: ", out_filename);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    output = &outfile;
    if(out_type == C_UNKNOWN) out_type = compression_from_filename(out_filename);
  }

  compression_type type = decompress ? in_type : out_type;
  buffer cbuf;

  switch(type) {
    case C_GZ:
      if(decompress)
        buffer_inflate(&cbuf, input);
      else
        buffer_deflate(&cbuf, output, level);
      break;
    case C_BZ2:
      buffer_bz2(&cbuf, decompress ? input : output, decompress ? 0 : level);
      break;
    case C_XZ:
    case C_LZMA:
      buffer_lzma(&cbuf, decompress ? input : output, decompress ? 0 : level);
      break;
    default:
      buffer_putm(buffer_2, "ERROR: Unable to detect compression type from ", in_filename);
      buffer_putnlflush(buffer_2);
      exit(EXIT_FAILURE);
  }

  if(decompress == 0 && output == buffer_1) {
    buffer_putsflush(buffer_2, "ERROR: Won't write compressed data to a terminal\n");
    exit(EXIT_FAILURE);
  }

  if(decompress) {
    buffer_copy(output, &cbuf);
    buffer_flush(output);
  } else {
    buffer_copy(&cbuf, input);
    buffer_flush(&cbuf);
  }

  return 0;
}
