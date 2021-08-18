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
#include "lib/unix.h"

static uint32 mask;

void
putdata(const uint8* x, size_t n) {
  size_t i;
  for(i = 0; i < n; i++) {
    if(i)
      buffer_putspace(buffer_2);
    buffer_putxlong0(buffer_2, x[i], 2);
  }
}

uint32
mem_top(ihex_file* h) {
  ihex_record* r;
  uint32 top = 0;
  ihex_addr a = {0};
  for(r = h->records; r; r = r->next) {
    if(r->type == 4) {
      uint16_unpack_big(r->data, &a.hi16);
    } else {
      a.lo16 = r->offset;
    }
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
    if(r->type == 4) {
      uint16_unpack_big(r->data, &a.hi16);
    } else {
      a.lo16 = r->offset;
    }
    if(a.off32 & (~mask))
      break;
    if(bottom > a.off32)
      bottom = a.off32;
  }
  return bottom;
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
                       " <hex-file> <cfgdata-file>\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "  -b, --bits N              max address bits\n"
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
      case 'b': scan_int(unix_optarg, &bits); break;
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
  buffer_puts(buffer_2, "Mask ");
  buffer_putxlong0(buffer_2, mask, 8);
  buffer_putsflush(buffer_2, "\n");

  if(unix_optind < argc) {
    hexfile = argv[unix_optind++];
  }
  static stralloc sa;
  size_t sz = 0;
  const char* x = mmap_read(hexfile, &sz);

  buffer_mmapread(&input, hexfile);
  buffer_getline_sa(&input, &sa);

  {
    ihex_record* r;
    ihex_file ihx;
    ihex_addr a = {0}, prev = {0};

    ret = ihex_record_read(&r, sa.s, sa.len);
    ret = ihex_read_buf(&ihx, x, sz);

    uint32 top = mem_top(&ihx);
    uint32 bottom = mem_bottom(&ihx);
    buffer_puts(buffer_2, "mem top = 0x");
    buffer_putxlong0(buffer_2, top, 4);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "mem bottom = 0x");
    buffer_putxlong0(buffer_2, bottom, 4);
    buffer_putnlflush(buffer_2);

    uint8_t* m = alloc_zero(top);
    buffer_truncfile(&output, "testihex.hex");

    for(r = ihx.records; r; r = r->next) {

      if(r->type == 4) {
        uint16_unpack_big(r->data, &a.hi16);
      } else {
        a.lo16 = r->offset;

        if(prev.off32 < a.off32) {
          buffer_puts(buffer_2, "empty space = 0x");
          buffer_putxlong0(buffer_2, prev.off32, 6);
          buffer_puts(buffer_2, ", len = 0x");
          buffer_putxlong0(buffer_2, a.off32 - prev.off32, 6);
          buffer_putnlflush(buffer_2);
        }
      }

      if(r->type == 0) {
        if((a.off32 & (~mask)))
          break;

        if((a.off32 & (~mask)) == 0) {
          ihex_record_write(r, &output);
          byte_copy(&m[a.off32], r->length, r->data);
        }

        buffer_putxlong0(buffer_2, a.off32, 8);
        buffer_putm_internal(buffer_2, " ", ihex_typestr(r->type), "(", 0);
        buffer_putulong(buffer_2, r->length);
        buffer_puts(buffer_2, ") ");
        putdata(r->data, r->length);
        buffer_putnlflush(buffer_2);
      }

      prev = a;
      prev.off32 += r->length;
    }
    /* r = ihex_record_insert(&ihx, 0x004000, 16);
     byte_zero(r->data, 16);
     byte_copy(r->data, 8, "\xde\xad\xca\xfe\xba\xbe\xf0\x0f");
     r->checksum = ihex_record_checksum(r);*/
    buffer_close(&output);

    int fd = open_trunc("testihex.bin");
    write(fd, m, top);
    close(fd);
  }

  buffer_puts(buffer_2, "ret = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
}
