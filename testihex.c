#include "lib/ihex.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/byte.h"
//#include "lib/slist.h"
#include "lib/uint32.h"
#include "lib/uint8.h"
#include "lib/str.h"
#include "lib/path.h"
#include "lib/scan.h"
#include "lib/errmsg.h"
#include "lib/unix.h"

static uint32 mask;
static ihex_file hex_output;

static void
putdata(buffer* out, const uint8* x, size_t n) {
  size_t i;
  for(i = 0; i < n; i++) {
    if(i)
      buffer_putspace(out);
    buffer_putxlong0(out, x[i], 2);
  }
}

static void
puthex(const char* name, uint32 value) {
  buffer_putm_internal(buffer_2, name, " = 0x", 0);
  buffer_putxlong0(buffer_2, value, 4);
  buffer_putnlflush(buffer_2);
}

uint32
mem_top(ihex_file* h, uint32 mask) {
  ihex_record* r;
  uint32 top = 0;
  ihex_addr a = {0};
  for(r = h->records; r; r = r->next) {
    if(r->type == 4)
      uint16_unpack_big(r->data, &a.hi16);
    else
      a.lo16 = r->offset;
    if(a.off32 & (~mask))
      break;
    if(top < a.off32 + r->length)
      top = a.off32 + r->length;
  }
  return top;
}

uint32
mem_bottom(ihex_file* h) {
  ihex_record* r;
  uint32 bottom = 0xffffffff;
  ihex_addr a = {0};
  for(r = h->records; r; r = r->next) {
    if(r->type == 4)
      uint16_unpack_big(r->data, &a.hi16);
    else
      a.lo16 = r->offset;
    if(a.off32 & (~mask))
      break;
    if(bottom > a.off32)
      bottom = a.off32;
  }
  return bottom;
}

int
hex_load(ihex_file* hex, const char* filename) {
  static stralloc sa;
  int ret = -1;
  size_t sz = 0;
  const char* x;
  if((x = mmap_read(filename, &sz))) {
    ihex_record* r;
    buffer input;
    buffer_mmapread(&input, filename);
    buffer_getline_sa(&input, &sa);
    ret = ihex_record_read(&r, sa.s, sa.len);
    ret = ihex_read_buf(hex, x, sz);
    buffer_close(&input);
    mmap_unmap(x, sz);
    ret = 0;
  }
  return ret;
}

int
hex_copy(ihex_record* r, uint8_t* m) {
  ihex_addr a = {0}, prev = {0};
  for(; r; r = r->next) {
    ihex_record_address(r, &a);

    if(r->type == 0) {
      if((a.off32 & (~mask)))
        break;
      if((a.off32 & (~mask)) == 0)
        byte_copy(&m[a.off32], r->length, r->data);
    }
    prev = a;
    prev.off32 += r->length;
  }
}

int
hex_save(ihex_record* r, const char* filename) {
  ihex_addr a = {0}, prev = {0};
  buffer output;
  buffer_truncfile(&output, filename);
  for(; r; r = r->next) {
    ihex_record_address(r, &a);
    if(r->type == 0) {
      if((a.off32 & (~mask)))
        break;
      if((a.off32 & (~mask)) == 0)
        ihex_record_write(r, &output);
    }
    prev = a;
    prev.off32 += r->length;
  }
  buffer_close(&output);
}

int
hex_print(ihex_record* r, buffer* out) {
  ihex_addr a = {0}, prev = {0};
  for(; r; r = r->next) {
    if(ihex_record_address(r, &a)) {
      a.lo16 = r->offset;
      if(prev.off32 < a.off32) {
        buffer_puts(out, "empty space = 0x");
        buffer_putxlong0(out, prev.off32, 6);
        buffer_puts(out, ", len = 0x");
        buffer_putxlong0(out, a.off32 - prev.off32, 6);
        buffer_putnlflush(out);
      }
    }

    if(r->type == 0) {
      if((a.off32 & (~mask)))
        break;
      buffer_putxlong0(out, a.off32, 8);
      buffer_putm_internal(out, " ", ihex_typestr(r->type), "(", 0);
      buffer_putulong(out, r->length);
      buffer_puts(out, ") ");
      putdata(out, r->data, r->length);
      buffer_putnlflush(out);
    }

    prev = a;
    prev.off32 += r->length;
  }
}

/**
 * @brief usage  Show command line usage
 * @param argv0
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       path_basename(argv0),
                       " <hex-files...>\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "  -b, --bits N              max address bits\n"
                       "  -o, --output FILE         combine into output file\n"
                       "  -v, --verbose             show verbose messages\n"
                       "\n",
                       0);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  ssize_t ret;
  static buffer input, output;
  int verbose = 0, bits = 32;
  const char* filename = argv[1] ? argv[1]
                                 : "/home/roman/Dokumente/Sources/xc8/pictest/bootloaders/18f2550-usb-hid-xc8/FIRMWARE/"
                                   "PIC18F2550/18F2550-MPLAB.X/dist/default/production/18F2550-MPLAB.X.production.hex";
  int c, index = 0;
  const char *cfgdata = 0, *hexfile = 0;

  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"bits", 0, NULL, 'b'},
                                {"verbose", 0, &verbose, 1},
                                {0, 0, 0, 0}};

  for(;;) {
    c = unix_getopt_long(argc, argv, "hvb:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'b': {
        scan_int(argv[unix_optind], &bits);
        break;
      }
      case 'h': usage(argv[0]); return 0;
      case 'v': verbose++; break;
      default:
        buffer_puts(buffer_2, "No such option '-");
        buffer_putc(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");

        return 1;
    }
  }

  mask = (1ull << bits) - 1ll;

  puthex("mask", mask);

  if(unix_optind < argc) {
    hexfile = argv[unix_optind++];
  }

  {
    ihex_record* r;
    ihex_file ihx;
    ihex_addr a = {0}, prev = {0};
    uint8_t* m;
    uint32 top, bottom;

    if(hex_load(&ihx, hexfile)) {
      errmsg_warnsys("hex_load: ", 0);
      return 1;
    }
    bottom = mem_bottom(&ihx);
    top = mem_top(&ihx, mask);

    puthex("mem bottom", bottom);
    puthex("mem top", top);

    for(r = ihx.records; r; r = r->next) {
      if(ihex_record_address(r, &a)) {
      }
    }

    m = alloc(top);
    byte_fill(m, top, 0xff);

    hex_print(ihx.records, buffer_2);
    hex_copy(ihx.records, m);
    hex_save(ihx.records, "testihex.hex");

    int fd = open_trunc("testihex.bin");
    write(fd, m, top);
    close(fd);
  }

  buffer_puts(buffer_2, "ret = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
}
