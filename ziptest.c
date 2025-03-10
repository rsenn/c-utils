#include "lib/unix.h"
#include "lib/buffer.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/mmap.h"
#include "lib/open.h"

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
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       argv0,
                       " [-o output] [infile or                        tdin]\n\n",
                       "  -1 ... -9                                  ompression level; default is                        \n",
                       "\n",
                       "Supported types are:",
                       NULL);
#ifdef HAVE_ZLIB
  buffer_puts(buffer_1, " gz");
#endif
#ifdef HAVE_LIBBZ2
  buffer_puts(buffer_1, " bz2");
#endif
#ifdef HAVE_LIBLZMA
  buffer_puts(buffer_1, " lzma xz");
#endif
  buffer_puts(buffer_1, "\n");
  buffer_flush(buffer_1);
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
  if(str_case_equal(ext, "gz") || str_case_equal(ext, "tgz"))
    return C_GZ;

  if(str_case_equal(ext, "bz2") || str_case_equal(ext, "tbz2") || str_case_equal(ext, "tbz"))
    return C_BZ2;

  if(str_case_equal(ext, "xz") || str_case_equal(ext, "txz"))
    return C_XZ;

  if(str_case_equal(ext, "lzma"))
    return C_LZMA;

  return C_UNKNOWN;
}

compression_type
compression_from_filename(const char* fn) {
  const char* ext = fn + str_rchr(fn, '.');

  if(*ext)
    return compression_from_ext(++ext);
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
  buffer infile, outfile;
  buffer *input, *output;
  compression_type type;
  buffer cbuf;

  while((opt = unix_getopt(argc, argv, "123456789dt:o:h")) != -1) {
    switch(opt) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': level = opt - '0'; break;
      case 'd': decompress = 1; break;
      case 't': in_type = compression_from_ext(unix_optarg); break;
      case 'o': out_filename = unix_optarg; break;
      case 'h': usage(str_basename(argv[0])); exit(EXIT_SUCCESS);
      default: /* '?' */ buffer_putm_internal(buffer_2, "Usage: ", argv[0], "[-t TYPE] [-o OUTPUT]                              file]\n", NULL); exit(EXIT_FAILURE);
    }
  }

  if(argv[unix_optind])
    in_filename = argv[unix_optind];

  if(str_equal(in_filename, "-")) {
    input = buffer_0;
  } else {
    if(buffer_mmapprivate(&infile, in_filename) < 0) {
      buffer_putm_internal(buffer_2, "ERROR opening: ", in_filename, NULL);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    input = &infile;

    if(in_type == C_UNKNOWN) {
      in_type = compression_from_filename(in_filename);

      if(in_type != C_UNKNOWN)
        decompress = 1;
    }
  }

  if(str_equal(out_filename, "-")) {
    output = buffer_1;
  } else {
    if(buffer_truncfile(&outfile, out_filename) < 0) {
      buffer_putm_internal(buffer_2, "ERROR opening: ", out_filename, NULL);
      buffer_putnlflush(buffer_2);
      return 1;
    }
    output = &outfile;

    if(out_type == C_UNKNOWN)
      out_type = compression_from_filename(out_filename);
  }

  type = decompress ? in_type : out_type;

  switch(type) {
    case C_GZ:
      if(decompress) {
        if(input == buffer_0)
          buffer_gunzip_fd(&cbuf, STDIN_FILENO);
        else
          buffer_gunzip(&cbuf, in_filename);
      } else {
        if(output == buffer_1)
          buffer_gzip_fd(&cbuf, STDOUT_FILENO, level);
        else
          buffer_gzip(&cbuf, out_filename, level);
      }
      break;
#if 0
    case C_BZ2:
      if(decompress) {
        if(input == buffer_0)
          buffer_bunzip_fd(&cbuf, STDIN_FILENO);
        else
          buffer_bunzip(&cbuf, in_filename);
      } else {
        if(output == buffer_1)
          buffer_bzip_fd(&cbuf, STDOUT_FILENO, level);
        else
          buffer_bzip(&cbuf, out_filename, level);
      }
      break;
#endif
    /*    case C_XZ: */
    /*    case C_LZMA: */
    /*      buffer_lzma(&cbuf,
     * decompress ? input : output,
     * decompress ? 0 : level); */
    /*      break; */
    default:
      buffer_putm_internal(buffer_2, "ERROR: Unable to detect                            ompression type from ", in_filename, NULL);
      buffer_putnlflush(buffer_2);
      exit(EXIT_FAILURE);
  }

  if(decompress == 0 && output == buffer_1) {
    buffer_putsflush(buffer_2, "ERROR: Won't write compressed                      ata to a terminal\n");
    exit(EXIT_FAILURE);
  }

  if(decompress) {
    buffer_copy(output, &cbuf);
    buffer_flush(output);
    buffer_close(output);
  } else {
    buffer_copy(&cbuf, input);
    buffer_flush(&cbuf);
    buffer_close(&cbuf);
  }

  return 0;
}
