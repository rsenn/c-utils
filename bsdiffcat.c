/*
 * https://www.daemonology.net/bsdiff/
 *
 * https://www.daemonology.net/papers/bsdiff.pdf
 *
 * https://www.daemonology.net/papers/thesis.pdf
 */

#include "lib/windoze.h"
#include "lib/array.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/path.h"
#include "lib/errmsg.h"
#include "lib/uint32.h"
#include "lib/uint8.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

typedef struct {
  char magic[8];
  int64 ctrl_len;
  int64 data_len;
  int64 new_size;
} bsdiff_header;

typedef struct {
  int64 add_len;
  int64 extra_len;
  int64 seek_off;
} bsdiff_control;

static buffer old, new;

static void
output_hex(const char* x, int64 n, int offset, char space) {
  int64 i;

  for(i = 0; i < n; i += 16) {
    size_t j;
    uint64 r = n - i < 16 ? n - i : 16;

    if(n <= 16 || byte_count(&x[i], r, '\0') < r) {
      buffer_puts(buffer_1, "0x");
      buffer_putxlong0(buffer_1, offset + i, 8);
      buffer_putnspace(buffer_1, 2);

      for(j = 0; j < r; ++j) {
        buffer_PUTC(buffer_1, space);
        buffer_putxlong0(buffer_1, (uint8)x[i + j], 2);
      }

      buffer_putnlflush(buffer_1);
    }
  }
}

static void
debug_int(const char* name, int64 value) {
  buffer_puts(buffer_1, name);
  buffer_puts(buffer_1, ": ");
  buffer_putlonglong(buffer_1, value);
  buffer_putnlflush(buffer_1);
}

static void
debug_hex(const char* name, int64 value) {
  buffer_puts(buffer_1, name);
  buffer_puts(buffer_1, ": 0x");
  buffer_putxlonglong0(buffer_1, value, 8);
  buffer_puts(buffer_1, " (");
  buffer_putlonglong(buffer_1, value);
  buffer_puts(buffer_1, ")");
  buffer_putnlflush(buffer_1);
}

int
buffer_getint64(buffer* b, int64* i) {
  char buffer[8];
  uint64 u;

  if(buffer_get(b, buffer, 8) != 8)
    return 0;

  uint64_unpack(buffer, &u);

  *i = (u & 0x7fffffffffffffff);

  if(u & 0x8000000000000000)
    *i = -*i;

  return 1;
}

static buffer
buffer_offset(const buffer* from, int64 offset) {
  buffer r = *from;

  r.p += offset;

  return r;
}

int
bsdiff_read_header(buffer* b, bsdiff_header* hdr) {
  char buffer[8];

  if(buffer_get(b, hdr->magic, 8) != 8)
    return 0;

  if(byte_diff(hdr->magic, 7, "BSDIFF4"))
    return 0;

  if(!buffer_getint64(b, &hdr->ctrl_len))
    return 0;

  if(!buffer_getint64(b, &hdr->data_len))
    return 0;

  if(!buffer_getint64(b, &hdr->new_size))
    return 0;

  return 1;
}

int
bsdiff_read_ctrl(buffer* b, bsdiff_control* ctrl) {
  byte_zero(ctrl, sizeof(ctrl));

  if(!buffer_getint64(b, &ctrl->add_len))
    return 0;

  if(!buffer_getint64(b, &ctrl->extra_len))
    return 0;

  if(!buffer_getint64(b, &ctrl->seek_off))
    return 0;

  return 1;
}

int64
bsdiff_dump_ctrl(const bsdiff_control ctrl) {
  debug_int("control.add_len", ctrl.add_len);
  debug_int("control.extra_len", ctrl.extra_len);
  debug_int("control.seek_off", ctrl.seek_off);
  debug_hex("control.offset", ctrl.add_len + ctrl.seek_off);
}

int64
bsdiff_dump_header(const bsdiff_header h) {
  debug_int("header.ctrl_len", h.ctrl_len);
  debug_int("header.data_len", h.data_len);
  debug_int("header.new_size", h.new_size);
}

int64
bsdiff_read(buffer* ctrl) {
  bsdiff_header h = {0};
  bsdiff_control rec = {0};
  buffer data, extra, bctrl, bdata, bextra;
  int64 i, r = 0, w = 0;

  if(!bsdiff_read_header(ctrl, &h)) {
    buffer_puts(buffer_2, "ERROR: could not read bsdiff header");
    buffer_putnlflush(buffer_2);
    return -1;
  }

  bsdiff_dump_header(h);

  data = buffer_offset(ctrl, h.ctrl_len);
  extra = buffer_offset(&data, h.data_len);

  buffer_bz2(&bctrl, ctrl, 0);
  buffer_bz2(&bdata, &data, 0);
  buffer_bz2(&bextra, &extra, 0);

  for(;;) {
    int64 len, j;
    char *add = 0, *src = 0, *extra = 0;

    if(!bsdiff_read_ctrl(&bctrl, &rec))
      break;

    bsdiff_dump_ctrl(rec);

    if((len = rec.add_len)) {
      add = malloc(len);

      if(buffer_get(&bdata, add, len) != len) {
        free(add);

        buffer_puts(buffer_2, "ERROR: could not get ");
        buffer_putulonglong(buffer_2, len);
        buffer_puts(buffer_2, " bytes from BDATA record");
        buffer_putnlflush(buffer_2);

        break;
      }

      if(old.x) {
        src = malloc(len);

        if(buffer_get(&old, src, len) != len) {
          free(add);
          free(src);
          break;
        }

        for(j = 0; j < len; ++j) {
          uint8 from = src[j], to = src[j] += add[j];

          if(add[j] && !new.x) {
            buffer_puts(buffer_1, "  patch(0x");
            buffer_putxlonglong0(buffer_1, w + j, 8);
            buffer_puts(buffer_1, ", 0x");
            buffer_putxlong0(buffer_1, from, 2);
            buffer_puts(buffer_1, ", 0x");
            buffer_putxlong0(buffer_1, to, 2);
            buffer_puts(buffer_1, ");");
            buffer_putnlflush(buffer_1);
          }
        }

        if(new.x)
          buffer_put(&new, src, len);

        free(src);
      } else {
        output_hex(add, len, r, '+');
      }

      free(add);

      r += len;
      w += len;
    }

    if((len = rec.extra_len)) {
      extra = malloc(len);

      if(buffer_get(&bextra, extra, len) != len) {
        free(extra);
        break;
      }

      if(new.x)
        buffer_put(&new, extra, len);

      if(!old.x)
        output_hex(extra, len, w, ' ');

      free(extra);

      w += len;
    }

    r += rec.seek_off;

    if(old.x)
      old.p += rec.seek_off;
  }

  if(old.x) {
    if(new.x) {
      buffer_flush(&new);
      buffer_close(&new);
    }

    buffer_close(&old);
  }

  buffer_close(&bctrl);
  buffer_close(&bdata);
  buffer_close(&bextra);

  return r;
}

int
main(int argc, char* argv[]) {
  buffer patch;
  bsdiff_header h;
  array records;
  int exitcode = 0;

  errmsg_iam(argv[0]);

  array_init(&records);

  if(argc <= 1 || buffer_mmapread(&patch, argv[1])) {
    errmsg_infosys(path_basename(argv[1]), ": ", "open file", ": ", NULL);
    return 1;
  }

  if(argc > 2) {
    if(buffer_mmapread(&old, argv[2]))
      byte_zero(&old, sizeof(old));

    if(argc > 3)
      if(buffer_truncfile(&new, argv[3]))
        byte_zero(&new, sizeof(new));
  }

  /*if(!old.x && !isatty(buffer_0->fd))
    if(buffer_mmapread_fd(&old, buffer_0->fd))
      byte_zero(&old, sizeof(old));*/

  /*if(!new.x && !isatty(buffer_1->fd)) {
    new = *buffer_1;
    buffer_1 = buffer_2;
  }*/

  if(bsdiff_read(&patch) == -1) {
    errmsg_infosys(path_basename(argv[0]), ": ", "read header", ": ", NULL);
    exitcode = 2;
  }

  buffer_close(&patch);
  return exitcode;
}
