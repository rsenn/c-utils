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
#include "lib/errmsg.h"
#include "lib/uint32.h"

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

static buffer old;
static buffer new;

void
output_hex(const char* x, int64 n, int offset, char space) {
  int64 i;

  for(i = 0; i < n; i += 16) {
    int j;
    int64 r = n - i < 16 ? n - i : 16;

    if(n <= 16 || byte_count(&x[i], r, '\0') < r) {

      buffer_putxlong0(buffer_1, offset + i, 8);
      buffer_putnspace(buffer_1, 2);

      for(j = 0; j < r; ++j) {
        buffer_PUTC(buffer_1, space);
        buffer_putxlong0(buffer_1, (long)(unsigned long)(unsigned char)x[i + j], 2);
      }
      buffer_putnlflush(buffer_1);
    }
  }
}

void
debug_int(const char* name, int64 value) {
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
  if(buffer_get(b, buffer, 8) != 8) return 0;
  uint64_unpack(buffer, &u);

  *i = (u & 0x7fffffffffffffff);

  if(u & 0x8000000000000000) *i = -*i;

  return 1;
}

void
buffer_offset(buffer* from, buffer* to, int64 offset) {
  buffer_init(to, from->op, from->fd, from->x, from->a);
  to->n = from->n;
  to->p = from->p + offset;
}

int
bsdiff_read_header(buffer* b, bsdiff_header* hdr) {
  char buffer[8];

  if(buffer_get(b, hdr->magic, 8) != 8) return 0;

  if(byte_diff(hdr->magic, 7, "BSDIFF4")) return 0;

  if(!buffer_getint64(b, &hdr->ctrl_len)) return 0;
  if(!buffer_getint64(b, &hdr->data_len)) return 0;
  if(!buffer_getint64(b, &hdr->new_size)) return 0;

  return 1;
}

int
bsdiff_read_ctrl(buffer* b, bsdiff_control* ctrl) {
  byte_zero(ctrl, sizeof(ctrl));

  if(!buffer_getint64(b, &ctrl->add_len)) return 0;
  if(!buffer_getint64(b, &ctrl->extra_len)) return 0;
  if(!buffer_getint64(b, &ctrl->seek_off)) return 0;
  return 1;
}

int64
bsdiff_read(buffer* ctrl, buffer* data, buffer* extra) {
  bsdiff_control rec;
  buffer bctrl, bdata, bextra;
  int64 i;
  int64 r = 0, w = 0;

  buffer_bz2(&bctrl, ctrl, 0);
  buffer_bz2(&bdata, data, 0);
  buffer_bz2(&bextra, extra, 0);

  for(;;) {
    uint64 len;
    if(!bsdiff_read_ctrl(&bctrl, &rec)) break;


    debug_int("add_len", rec.add_len);
    debug_int("extra_len", rec.extra_len);
    debug_int("seek_off", rec.seek_off);

    if((len = rec.add_len)) {
      char* add = malloc(len);

      if(buffer_get(&bdata, add, len) != len) {
        free(add);
        break;
      }
      if(!old.x)
        output_hex(add, len, r, '+');

      if(old.x) {
        char* src = malloc(len);
        int64 j;
        if(buffer_get(&old, src, len) != len) {
          free(add);
          free(src);
          break;
        }
        for(j = 0; j < len; ++j) {

          char to, from = src[j];

          to = add[j] += src[j];

          if(from != to && !new.x) {
            buffer_puts(buffer_1, "  patch(p, 0x");
            buffer_putxlonglong0(buffer_1, w + j, 8);
            buffer_puts(buffer_1, ", 0x");
            buffer_putxlong0(buffer_1, (unsigned long)(unsigned char)from, 2);
            buffer_puts(buffer_1, ", 0x");
            buffer_putxlong0(buffer_1, (unsigned long)(unsigned char)to, 2);
            buffer_puts(buffer_1, ");");
            buffer_putnlflush(buffer_1);
          }
        }
        free(src);

        if(new.x) {
          buffer_put(&new, add, len);
        }
      }

      free(add);
      r += len;
      w += len;
    }

    if((len = rec.extra_len)) {
      char* extra = malloc(len);
      if(buffer_get(&bextra, extra, len) != len) {
        free(extra);
        break;
      }

      if(new.x) buffer_put(&new, extra, len);

      if(!old.x)
        output_hex(extra, len, w, ' ');
      free(extra);

      w += len;
    }

    r += rec.seek_off;
    if(old.x) old.p += rec.seek_off;
  }

  if(old.x) {
    if(new.x) {
      buffer_flush(&new);
      buffer_close(&new);
    }
    buffer_close(&old);
  }

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

  if(argc > 2) {
    if(buffer_mmapread(&old, argv[2])) byte_zero(&old, sizeof(old));
    if(argc > 3) {
      if(buffer_truncfile(&new, argv[3])) byte_zero(&new, sizeof(new));
    }
  }

  if(!old.x && !isatty(buffer_0->fd)) {
    if(buffer_mmapread_fd(&old, buffer_0->fd)) byte_zero(&old, sizeof(old));
  }

  if(!new.x && !isatty(buffer_1->fd)) {
    new = *buffer_1;
    buffer_1 = buffer_2;
  }

  if(buffer_mmapread(&patch, argv[1]) == 0) {

    if(bsdiff_read_header(&patch, &h)) {

      buffer data, extra;

      buffer_offset(&patch, &data, h.ctrl_len);
      buffer_offset(&patch, &extra, h.ctrl_len + h.data_len);

      debug_int("ctrl_len", h.ctrl_len);
      debug_int("data_len", h.data_len);
      debug_int("new_size", h.new_size);

//      int64 n = bsdiff_read_ctrl(&patch, &records);
//      debug_int("n", n);

      bsdiff_read(&patch, &data, &extra);

    } else {
      errmsg_infosys(str_basename(argv[0]), ": ", "read header", ": ", NULL);
      exitcode = 2;
    }
  } else {
    errmsg_infosys(str_basename(argv[0]), ": ", "open file", ": ", NULL);
    exitcode = 1;
  }

  buffer_close(&patch);
  return exitcode;
}
