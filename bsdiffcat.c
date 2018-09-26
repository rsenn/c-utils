#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/errmsg.h"
#include "lib/uint32.h"
#include "lib/uint64.h"

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
        buffer_putc(buffer_1, space);
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
  buffer_putint64(buffer_1, value);
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
  buffer_init(to, (void*)from->op, from->fd, from->x, from->a);
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

int64
bsdiff_read_ctrl(buffer* b, array* a) {
  bsdiff_control ctrl;
  buffer bctrl;
  buffer_bz2(&bctrl, b, 0);

  for(;;) {
    byte_zero(&ctrl, sizeof(ctrl));

    if(!buffer_getint64(&bctrl, &ctrl.add_len)) break;
    if(!buffer_getint64(&bctrl, &ctrl.extra_len)) break;
    if(!buffer_getint64(&bctrl, &ctrl.seek_off)) break;

    array_catb(a, &ctrl, sizeof(ctrl));
  }
  return array_length(a, sizeof(ctrl));
}

int64
bsdiff_read_data(buffer* data, buffer* extra, bsdiff_control* rec, int64 n) {
  buffer bdata, bextra;
  int64 i;
  int64 r = 0, w = 0;

  buffer_bz2(&bdata, data, 0);
  buffer_bz2(&bextra, extra, 0);

  for(i = 0; i < n; ++i) {
    uint64 dlen = rec[i].add_len;

    debug_int("add_len", rec[i].add_len);
    debug_int("extra_len", rec[i].extra_len);
    debug_int("seek_off", rec[i].seek_off);

    if(dlen) {
      char* ptr = malloc(dlen);

      if(buffer_get(&bdata, ptr, dlen) != dlen) {
        free(ptr);
        break;
      }
      output_hex(ptr, dlen, r, '+');

      if(old.x) {
        char* src = malloc(dlen);
        int64 j;
        if(buffer_get(&old, src, dlen) != dlen) {
          free(ptr);
          free(src);
          break;
        }
        for(j = 0; j < dlen; ++j) {
          ptr[j] += src[j];
        }
          free(src);

        if(new.x) {
          buffer_put(&new, ptr, dlen);
        }
      }

      free(ptr);
      r += dlen;
      w += dlen;
    }

    if(rec[i].extra_len) {
      char* ptr = malloc(rec[i].extra_len);
      if(buffer_get(&bextra, ptr, rec[i].extra_len) != rec[i].extra_len) {
        free(ptr);
        break;
      }

      if(new.x) buffer_put(&new, ptr, rec[i].extra_len);

      output_hex(ptr, rec[i].extra_len, w, ' ');
      free(ptr);

      w += rec[i].extra_len;
    }

    r += rec[i].seek_off;
    if(old.x) old.p += rec[i].seek_off;
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

  if(!new.x) {
    if(isatty(buffer_1->fd)) {
      buffer_putsflush(buffer_2, "ERROR: won't write binary data to terminal\n");
      return 1;
    }
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

      int64 n = bsdiff_read_ctrl(&patch, &records);
      debug_int("n", n);

      bsdiff_read_data(&data, &extra, array_start(&records), n);

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
