#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/array.h"
#include "lib/errmsg.h"
#include "lib/uint32.h"
#include "lib/uint64.h"

typedef struct {
  char magic[8];
  uint64 ctrl_len;
  uint64 data_len;
  uint64 new_size;
} bsdiff_header;

typedef struct {
  uint64 add_len;
  uint64 extra_len;
  uint64 seek_off;
} bsdiff_control;

void
output_hex(const char* x, int64 n) {
  int64 i;

  for(i = 0; i < n; i += 16) {
    int j;
    int64 r = n - i < 16 ? n - i : 16;

    if(byte_count(&x[i], r, '\0') < r) {

      buffer_putxlong0(buffer_1, i, 8);
      buffer_putnspace(buffer_1, 3);

      for(j = 0; j < r; ++j) {
        if(j) buffer_putspace(buffer_1);
        buffer_putxlong0(buffer_1, (long)(unsigned long)(unsigned char)x[i + j], 2);
      }
      buffer_putnlflush(buffer_1);
    }
  }
}

void
debug_int(const char* name, int64 value) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putint64(buffer_2, value);
  buffer_putnlflush(buffer_2);
}

int
buffer_getuint64(buffer* b, uint64* u) {
  char buffer[8];
  if(buffer_get(b, buffer, 8) != 8) return 0;
  uint64_unpack(buffer, u);

  if(*u & 0x8000000000000000)
    *(int64*)u = -(*(int64*)u & 0x7fffffffffffffff);

  return 1;
}

int
bsdiff_read_header(buffer* b, bsdiff_header* hdr) {
  char buffer[8];

  if(buffer_get(b, hdr->magic, 8) != 8) return 0;

  if(byte_diff(hdr->magic, 7, "BSDIFF4")) return 0;

  if(!buffer_getuint64(b, &hdr->ctrl_len)) return 0;
  if(!buffer_getuint64(b, &hdr->data_len)) return 0;
  if(!buffer_getuint64(b, &hdr->new_size)) return 0;

  return 1;
}

int64
bsdiff_read_ctrl(buffer* b, array* a) {
  bsdiff_control ctrl;
  buffer bctrl;
  buffer_bz2(&bctrl, b, 0);

  for(;;) { 
    byte_zero(&ctrl, sizeof(ctrl));

    if(!buffer_getuint64(&bctrl, &ctrl.add_len)) break;
    if(!buffer_getuint64(&bctrl, &ctrl.extra_len)) break;
    if(!buffer_getuint64(&bctrl, &ctrl.seek_off)) break;

    debug_int("add_len", ctrl.add_len);
    debug_int("extra_len", ctrl.extra_len);
    debug_int("seek_off", ctrl.seek_off);

    array_catb(a, &ctrl, sizeof(ctrl));
  }
  return array_length(a, sizeof(ctrl));
}

int64
bsdiff_read_data(buffer* b, bsdiff_control* rec, int64 n) {
  buffer bdata;
  int64 i;
  uint64 r = 0;

  buffer_bz2(&bdata, b, 0);

  for(i = 0; i < n; ++i) {
    uint64 dlen = rec[i].add_len;
    char* data = malloc(dlen);

    if(buffer_get(&bdata, data, dlen) != dlen) break;

    output_hex(data, dlen);

    r += dlen;
  }

  return r;
}

int
main(int argc, char* argv[]) {
  buffer input;
  bsdiff_header h;
  array records;
  int exitcode = 0;

  array_init(&records);

  if(buffer_mmapread(&input, argv[1]) == 0) {

    if(bsdiff_read_header(&input, &h)) {

      debug_int("ctrl_len", h.ctrl_len);
      debug_int("data_len", h.data_len);
      debug_int("new_size", h.new_size);

      int64 n = bsdiff_read_ctrl(&input, &records);
      debug_int("n", n);

      bsdiff_read_data(&input, array_start(&records), n);

    } else {
      errmsg_infosys(str_basename(argv[0]), ": ", "read header", ": ", NULL);
      exitcode = 2;
    }
  } else {
      errmsg_infosys(str_basename(argv[0]), ": ", "open file", ": ", NULL);
      exitcode = 1;
  }

  buffer_close(&input);
  return exitcode;
}
