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
#include "lib/alloc.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static uint32 mask;
static ihex_file hex_output;
typedef union {
  struct list_head* el;
  ihex_record* r;
} ptr;

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
  buffer_putm_internal(buffer_2, name, " = 0x", NULL);
  buffer_putxlong0(buffer_2, value, 4);
  buffer_putnlflush(buffer_2);
}

uint32
mem_top(ihex_file* h, uint32 mask) {
  ptr p;
  uint32 top = 0;
  ihex_addr a = {0};
  list_for_each(p.el, &h->records) {
    if(p.r->type == 4)
      uint16_unpack_big(p.r->data, &a.hi16);
    else
      a.lo16 = p.r->offset;
    if(a.off32 & (~mask))
      break;
    if(top < a.off32 + p.r->length)
      top = a.off32 + p.r->length;
  }
  return top;
}

uint32
mem_bottom(ihex_file* h) {
  ptr p;
  uint32 bottom = 0xffffffff;
  ihex_addr a = {0};
  list_for_each(p.el, &h->records) {
    if(p.r->type == 4)
      uint16_unpack_big(p.r->data, &a.hi16);
    else
      a.lo16 = p.r->offset;
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

  ihex_init(hex);

  if((x = mmap_read(filename, &sz))) {
    ihex_record* r;
    buffer input;
    buffer_mmapread(&input, filename);
    buffer_getline_sa(&input, &sa);
    ret = ihex_record_read(&r, sa.s, sa.len);
    ret = ihex_read_buf(hex, x, sz);

    buffer_putm_internal(buffer_2, "Loaded '", filename, "' ... ", NULL);
    buffer_putlong(buffer_2, ret);
    buffer_puts(buffer_2, " records");
    buffer_putnlflush(buffer_2);

    buffer_close(&input);
    mmap_unmap(x, sz);
    ret = 0;
  }
  return ret;
}

int
hex_copy(ihex_file* h, uint8_t* m) {
  ptr p;
  ihex_addr a = {0}, prev = {0};
  list_for_each(p.el, &h->records) {
    ihex_record_address(p.r, &a);

    if(p.r->type == 0) {
      if((a.off32 & (~mask)))
        break;
      if((a.off32 & (~mask)) == 0)
        byte_copy(&m[a.off32], p.r->length, p.r->data);
    }
    prev = a;
    prev.off32 += p.r->length;
  }
}

int
hex_save(ihex_file* ihf, const char* filename) {
  ptr p;
  ihex_addr a = {0};
  uint32 prev = 0;
  buffer output;
  buffer_truncfile(&output, filename);

  list_for_each(p.el, &ihf->records) {
    ihex_record_write(p.r, &output);

    if(ihex_record_address(p.r, &a) == 4)
      continue;

    /*  if((a.off32 & (~mask)))
        break;
*/
    /*if((a.off32 & (~mask)) == 0)*/

    prev = a.off32 + p.r->length;
  }
  buffer_close(&output);
}

int
hex_print(ihex_file* ihf, buffer* out) {
  ptr p;
  ihex_addr a = {0};
  uint32 prev = 0;
  int i = 0;
  list_for_each(p.el, &ihf->records) {
    int type = ihex_record_address(p.r, &a);

    if(prev < a.off32 && type == 0) {
      buffer_puts(out, "     EMPTY      0x");
      buffer_putxlong0u(out, prev, 8);
      buffer_puts(out, "   Size: ");
      buffer_putulong(out, a.off32 - prev);
      buffer_puts(out, " bytes [0x");
      buffer_putxlong0u(out, a.off32 - prev, 4);
      buffer_puts(out, "]");
      buffer_putnlflush(out);
    }
    buffer_puts(buffer_2, i >= 100 ? "#" : i >= 10 ? " #" : "  #");
    buffer_putulong(buffer_2, i++);
    buffer_putc(buffer_2, ' ');

    /*  ihex_record* extlinaddr;
      uint16 hi16 = 0;

      if((extlinaddr = ihex_record_prev(ihf, p.r, 4)))
        uint16_unpack_big(extlinaddr->data, &hi16);*/

    ihex_record_dump(p.r, a.hi16, buffer_2);

    if(type == 4)
      continue;

    if(p.r->type == 0) {
      a.lo16 = p.r->offset;
      /*  if((a.off32 & (~mask)))
         break;*/
      /*
      buffer_putxlong0(out, a.off32, 8);
      buffer_putm_internal(out, " ", ihex_typestr(p.r->type), "(", NULL);
      buffer_putulong(out, p.r->length);
      buffer_puts(out, ") ");
      putdata(out, p.r->data, p.r->length);
      buffer_putnlflush(out);*/
    }
    prev = a.off32 + p.r->length;
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
  const char *cfgdata = 0, *input_file = 0, *output_file = 0;

  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"bits", 0, NULL, 'b'},
                                {"output", 1, NULL, 'o'},
                                {"verbose", 0, &verbose, 1},
                                {0, 0, 0, 0}};

  for(;;) {
    c = unix_getopt_long(argc, argv, "hvb:o:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'o': {
        output_file = unix_optarg; // argv[unix_optind];
        break;
      }
      case 'b': scan_int(argv[unix_optind], &bits); break;

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
  uint8_t* m = 0;
  size_t msize = 0;

  ihex_init(&hex_output);

  while(unix_optind < argc) {
    union {
      struct list_head* el;
      ihex_record* r;
    } p;
    ihex_file hex_input;
    ihex_addr a = {0}, prev = {0};
    uint32 top, bottom;
    input_file = argv[unix_optind++];

    if(hex_load(&hex_input, input_file)) {
      errmsg_warnsys("hex_load: ", 0);
      return 1;
    }
    bottom = mem_bottom(&hex_input);
    top = mem_top(&hex_input, mask);
    buffer_putm_internal(buffer_2, "Input Hex File '", input_file, "':\n", NULL);
    hex_print(&hex_input, buffer_2);

    puthex("mem bottom", bottom);
    puthex("mem top", top);

    list_for_each(p.el, &hex_input.records) {
      if(ihex_record_address(p.r, &a) == 0) {
        /*buffer_puts(buffer_2, "Record @");
        buffer_putxlong0(buffer_2, a.off32, 4);
        buffer_puts(buffer_2, ": ");
        putdata(buffer_2, r->data, r->length);
        buffer_putnlflush(buffer_2);*/

        ihex_put(&hex_output, a.off32, p.r->data, p.r->length);
      }
    }

    if(m == 0) {
      m = alloc(top);
      byte_fill(m, top, 0xff);
      msize = top;
    } else {
      size_t n = MAX(msize, top) - msize;
      if(n) {
        alloc_re(&m, msize, msize + n);
        byte_fill(&m[msize], n, 0xff);
        msize += n;
      }
    }

    hex_copy(&hex_input, m);
  }

  int fd = open_trunc("testihex.bin");
  write(fd, m, msize);
  close(fd);

  if(output_file) {
    buffer_puts(buffer_2, "Output Hex File:\n");
    hex_print(&hex_output, buffer_2);

    hex_save(&hex_output, output_file);
    buffer_putm_internal(buffer_2, "Wrote '", output_file, "' ...", NULL);
    buffer_putnlflush(buffer_2);
  }
  /*  buffer_puts(buffer_2, "ret = ");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);*/
}
