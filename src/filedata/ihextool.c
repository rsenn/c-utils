#include "../../lib/ihex.h"
#include "../../lib/buffer.h"
#include "../../lib/stralloc.h"
#include "../../lib/mmap.h"
#include "../../lib/byte.h"
#include "../../lib/str.h"
#include "../../lib/scan.h"
#include "../../lib/errmsg.h"
#include "../../lib/unix.h"
#include "../../lib/alloc.h"

/* largest flat image a command builds, 64 MiB */
#define MAX_IMAGE (64ul << 20)

typedef struct {
  uint8 *image, *map;
  uint32 lo;
  size_t size;
} flat;

static const char* prog;
static unsigned long opt_addr, opt_len, opt_fill = 0xff, opt_width = 16;
static int have_addr, have_len;
static const char* opt_output = "-";
static const char *opt_vectors = "0x00,0x08,0x18", *opt_family = "pic18";

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " COMMAND [OPTIONS] FILE...\n"
                       "\n"
                       "Commands:\n"
                       "  info FILE...         records, checksums and the address ranges holding data\n"
                       "  records FILE...      every record, decoded\n"
                       "  dump FILE...         the data as hex and ascii, one line per 16 bytes\n"
                       "  merge FILE...        write one normalized hex file, later files win\n"
                       "                       (also: cut, extract)\n"
                       "  bin FILE...          write the data as a flat binary, gaps filled\n"
                       "  frombin FILE         write a binary file as hex at address -a\n"
                       "  vectors FILE         add GOTO stubs at the vectors that jump to vector+offset (-a), so one\n"
                       "                       hex works with and without a bootloader (PIC18), write normalized hex\n"
                       "  verify FILE...       check checksums and structure, exit 1 on a problem\n"
                       "  sum FILE...          byte count and sums of the data\n"
                       "\n"
                       "Options:\n"
                       "  -a, --addr ADDR      first address (dump, merge, bin, sum), load address (frombin)\n"
                       "  -l, --length LEN     number of bytes from -a\n"
                       "  -f, --fill BYTE      value for bytes without data in bin, default 0xff\n"
                       "  -w, --width N        bytes per data record in merge and frombin, default 16\n"
                       "      --vectors LIST   vectors for vectors, default 0x00,0x08,0x18\n"
                       "      --family NAME    chip family for vectors, only pic18\n"
                       "  -o, --output FILE    write to FILE instead of standard output\n"
                       "  -h, --help           show this help\n"
                       "\n"
                       "FILE - is standard input, numbers are decimal or 0x hex.\n",
                       NULL);
  buffer_flush(buffer_1);
}

static int
number(const char* s, unsigned long* v) {
  size_t n;

  if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    if(!(n = scan_xlong(s + 2, v)))
      return 0;
    return s[2 + n] == '\0';
  }
  if(!(n = scan_ulong(s, v)))
    return 0;
  return s[n] == '\0';
}

/* reads the options of a command, returns the index of the first file */
static int
options(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"addr", 1, NULL, 'a'},
                                             {"length", 1, NULL, 'l'},
                                             {"fill", 1, NULL, 'f'},
                                             {"width", 1, NULL, 'w'},
                                             {"output", 1, NULL, 'o'},
                                             {"vectors", 1, NULL, 'V'},
                                             {"family", 1, NULL, 'F'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0;

  unix_optind = 1;

  while((c = unix_getopt_long(argc, argv, "a:l:f:w:o:hV:F:", opts, &index)) != -1) {
    switch(c) {
      case 'a':
        if(!number(unix_optarg, &opt_addr)) {
          errmsg_warn("invalid address ", unix_optarg, 0);
          return -1;
        }
        have_addr = 1;
        break;
      case 'l':
        if(!number(unix_optarg, &opt_len)) {
          errmsg_warn("invalid length ", unix_optarg, 0);
          return -1;
        }
        have_len = 1;
        break;
      case 'f':
        if(!number(unix_optarg, &opt_fill) || opt_fill > 255) {
          errmsg_warn("invalid fill byte ", unix_optarg, 0);
          return -1;
        }
        break;
      case 'w':
        if(!number(unix_optarg, &opt_width) || opt_width < 1 || opt_width > 255) {
          errmsg_warn("invalid width ", unix_optarg, 0);
          return -1;
        }
        break;
      case 'o': opt_output = unix_optarg; break;
      case 'V': opt_vectors = unix_optarg; break;
      case 'F': opt_family = unix_optarg; break;
      case 'h': usage(prog); return -2;
      default: return -1;
    }
  }
  return unix_optind;
}

/* loads a hex file into h, *skipped is the number of lines that are no record */
static int
load(const char* name, ihex_file* h, size_t* skipped) {
  stralloc sa;
  const char* x;
  size_t n, lines = 0, pos;
  ssize_t records;
  char tmp[4096];
  int mapped = 0;

  ihex_init(h);
  stralloc_init(&sa);

  if(str_equal(name, "-")) {
    ssize_t r;

    while((r = buffer_get(buffer_0, tmp, sizeof(tmp))) > 0)
      stralloc_catb(&sa, tmp, (size_t)r);

    x = sa.s;
    n = sa.len;
  } else {
    if(!(x = mmap_read(name, &n)))
      return -1;
    mapped = 1;
  }

  for(pos = 0; pos < n;) {
    size_t len = byte_chrs(x + pos, n - pos, "\r\n", 2);

    if(len)
      lines++;
    pos += len;
    while(pos < n && (x[pos] == '\r' || x[pos] == '\n'))
      pos++;
  }

  records = x ? ihex_read_buf(h, x, n) : 0;
  *skipped = lines - (size_t)records;

  if(mapped)
    mmap_unmap(x, n);
  else
    stralloc_free(&sa);

  return 0;
}

static ihex_file*
load_all(int argc, char* argv[], int first) {
  ihex_file* files;
  size_t skipped;
  int i;

  if(first >= argc) {
    errmsg_warn("no input file", 0);
    return 0;
  }

  files = alloc_zero(sizeof(ihex_file) * (size_t)(argc - first));

  for(i = first; i < argc; i++)
    if(load(argv[i], &files[i - first], &skipped) < 0) {
      errmsg_warnsys("cannot read ", argv[i], 0);
      return 0;
    }

  return files;
}

/* puts the data of n files into one image, later files win */
static int
flat_open(flat* f, ihex_file* files, int n) {
  uint32 lo = 0xffffffffu, hi = 0, a, b;
  int i;

  for(i = 0; i < n; i++)
    if(ihex_range(&files[i], &a, &b)) {
      if(a < lo)
        lo = a;
      if(b > hi)
        hi = b;
    }

  if(hi == 0 && !have_addr) {
    errmsg_warn("no data records", 0);
    return -1;
  }

  if(have_addr)
    lo = (uint32)opt_addr;
  if(have_len)
    hi = lo + (uint32)opt_len;

  if(hi <= lo) {
    errmsg_warn("no data in the requested range", 0);
    return -1;
  }

  if((unsigned long)(hi - lo) > MAX_IMAGE) {
    errmsg_warn("the data spans more than 64 MiB, narrow it with -a and -l", 0);
    return -1;
  }

  f->lo = lo;
  f->size = hi - lo;
  f->image = alloc(f->size);
  f->map = alloc_zero(f->size);

  if(!f->image || !f->map) {
    errmsg_warn("out of memory", 0);
    return -1;
  }

  byte_fill(f->image, f->size, (unsigned char)opt_fill);

  for(i = 0; i < n; i++)
    ihex_copy_to(&files[i], lo, f->image, f->map, f->size);

  return 0;
}

/* finds the next run of bytes with data at or after *pos, returns its length */
static size_t
flat_run(const flat* f, size_t* pos) {
  size_t i = *pos, e;

  while(i < f->size && !f->map[i])
    i++;

  for(e = i; e < f->size && f->map[e];)
    e++;

  *pos = i;
  return e - i;
}

static int
output_open(buffer* b) {
  if(str_equal(opt_output, "-"))
    return 0;

  if(buffer_truncfile(b, opt_output) < 0) {
    errmsg_warnsys("cannot write ", opt_output, 0);
    return -1;
  }
  return 1;
}

static buffer*
output_buffer(buffer* b, int opened) {
  return opened ? b : buffer_1;
}

static void
output_close(buffer* b, int opened) {
  buffer_flush(output_buffer(b, opened));

  if(opened)
    buffer_close(b);
}

static void
put_addr(buffer* b, unsigned long v) {
  buffer_puts(b, "0x");
  buffer_putxlong0u(b, v, 8);
}

static int
cmd_info(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  int i;

  if(!files)
    return 1;

  for(i = first; i < argc; i++) {
    ihex_file* h = &files[i - first];
    ihex_recptr p;
    size_t records = 0, bytes, bad, pos = 0, run;
    uint32 lo = 0, hi = 0;
    int eof = 0;
    flat f;

    list_for_each(p.link, &h->records) {
      records++;
      if(p.rec->type == IHEX_EOF)
        eof = 1;
    }

    bytes = ihex_range(h, &lo, &hi);
    bad = ihex_verify(h);

    buffer_putm_internal(buffer_1, argv[i], ": ", NULL);
    buffer_putulong(buffer_1, records);
    buffer_puts(buffer_1, " records, ");
    buffer_putulong(buffer_1, bytes);
    buffer_puts(buffer_1, " data bytes, ");
    buffer_putulong(buffer_1, bad);
    buffer_puts(buffer_1, " checksum errors");
    buffer_puts(buffer_1, eof ? "" : ", no end of file record");
    buffer_putnlflush(buffer_1);

    if(!bytes)
      continue;

    buffer_puts(buffer_1, "  lowest ");
    put_addr(buffer_1, lo);
    buffer_puts(buffer_1, ", end ");
    put_addr(buffer_1, hi);
    buffer_putnlflush(buffer_1);

    if((unsigned long)(hi - lo) > MAX_IMAGE) {
      buffer_putsflush(buffer_1, "  (too wide to list the ranges)\n");
      continue;
    }

    f.lo = lo;
    f.size = hi - lo;
    f.image = alloc(f.size);
    f.map = alloc_zero(f.size);
    ihex_copy_to(h, lo, f.image, f.map, f.size);

    while((run = flat_run(&f, &pos))) {
      buffer_puts(buffer_1, "  ");
      put_addr(buffer_1, f.lo + pos);
      buffer_puts(buffer_1, " - ");
      put_addr(buffer_1, f.lo + pos + run - 1);
      buffer_puts(buffer_1, "  ");
      buffer_putulong(buffer_1, run);
      buffer_putsflush(buffer_1, " bytes\n");
      pos += run;
    }
  }
  return 0;
}

static int
cmd_records(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  int i;

  if(!files)
    return 1;

  for(i = first; i < argc; i++) {
    ihex_recptr p;
    uint32 base = 0;

    if(argc - first > 1)
      buffer_putm_internal(buffer_1, argv[i], ":\n", NULL);

    list_for_each(p.link, &files[i - first].records) {
      ihex_record_base(p.rec, &base);
      ihex_record_dump(p.rec, (uint16)(base >> 16), buffer_1);
    }
    buffer_flush(buffer_1);
  }
  return 0;
}

static int
cmd_dump(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  size_t pos = 0, run, i, n;
  buffer out, *b;
  int opened;
  flat f;

  if(!files || flat_open(&f, files, argc - first) < 0 || (opened = output_open(&out)) < 0)
    return 1;

  b = output_buffer(&out, opened);

  while((run = flat_run(&f, &pos))) {
    size_t end = pos + run;

    while(pos < end) {
      n = end - pos < 16 ? end - pos : 16;

      put_addr(b, f.lo + pos);
      buffer_puts(b, " ");

      for(i = 0; i < 16; i++) {
        buffer_putspace(b);
        if(i < n)
          buffer_putxlong0u(b, f.image[pos + i], 2);
        else
          buffer_puts(b, "  ");
      }

      buffer_puts(b, "  |");
      for(i = 0; i < n; i++)
        buffer_putc(b, f.image[pos + i] >= 32 && f.image[pos + i] < 127 ? f.image[pos + i] : '.');
      buffer_puts(b, "|\n");
      pos += n;
    }
  }
  output_close(&out, opened);
  return 0;
}

static int
cmd_merge(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  size_t pos = 0, run;
  uint16 hi16 = 0;
  buffer out, *b;
  int opened;
  flat f;

  if(!files || flat_open(&f, files, argc - first) < 0 || (opened = output_open(&out)) < 0)
    return 1;

  b = output_buffer(&out, opened);

  while((run = flat_run(&f, &pos))) {
    ihex_write_data(b, &hi16, f.lo + (uint32)pos, f.image + pos, run, (unsigned)opt_width);
    pos += run;
  }

  ihex_write_eof(b);
  output_close(&out, opened);
  return 0;
}

static int
cmd_bin(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  buffer out, *b;
  int opened;
  flat f;

  if(!files || flat_open(&f, files, argc - first) < 0 || (opened = output_open(&out)) < 0)
    return 1;

  b = output_buffer(&out, opened);
  buffer_put(b, (const char*)f.image, f.size);
  output_close(&out, opened);
  return 0;
}

static int
cmd_frombin(int argc, char* argv[], int first) {
  const char* x;
  size_t n;
  uint16 hi16 = 0;
  buffer out, *b;
  int opened;

  if(argc - first != 1) {
    errmsg_warn("frombin takes one binary file", 0);
    return 1;
  }

  if(!(x = mmap_read(argv[first], &n))) {
    errmsg_warnsys("cannot read ", argv[first], 0);
    return 1;
  }

  if((opened = output_open(&out)) < 0)
    return 1;

  b = output_buffer(&out, opened);
  ihex_write_data(
      b, &hi16, (uint32)opt_addr, (const uint8*)x, have_len && opt_len < n ? opt_len : n, (unsigned)opt_width);
  ihex_write_eof(b);
  output_close(&out, opened);
  mmap_unmap(x, n);
  return 0;
}

static int
cmd_verify(int argc, char* argv[], int first) {
  int i, status = 0;

  if(first >= argc) {
    errmsg_warn("no input file", 0);
    return 1;
  }

  for(i = first; i < argc; i++) {
    ihex_file h;
    ihex_recptr p;
    size_t skipped, bad, records = 0;
    int eof = 0, problems = 0;

    if(load(argv[i], &h, &skipped) < 0) {
      errmsg_warnsys("cannot read ", argv[i], 0);
      status = 1;
      continue;
    }

    list_for_each(p.link, &h.records) {
      records++;
      if(p.rec->type == IHEX_EOF)
        eof = 1;
    }
    bad = ihex_verify(&h);

    if(skipped) {
      buffer_putm_internal(buffer_1, argv[i], ": ", NULL);
      buffer_putulong(buffer_1, skipped);
      buffer_putsflush(buffer_1, " lines are no valid record\n");
      problems++;
    }
    if(bad) {
      buffer_putm_internal(buffer_1, argv[i], ": ", NULL);
      buffer_putulong(buffer_1, bad);
      buffer_putsflush(buffer_1, " records with a wrong checksum\n");
      problems++;
    }
    if(!eof) {
      buffer_putm_internal(buffer_1, argv[i], ": no end of file record\n", NULL);
      buffer_flush(buffer_1);
      problems++;
    }
    if(!problems) {
      buffer_putm_internal(buffer_1, argv[i], ": ok, ", NULL);
      buffer_putulong(buffer_1, records);
      buffer_putsflush(buffer_1, " records\n");
    } else {
      status = 1;
    }
  }
  return status;
}

/* GOTO target (PIC18, 4 bytes): word address k = target/2 */
static void
pic18_goto(uint8* x, unsigned long target) {
  unsigned long k = target / 2;

  x[0] = (uint8)(k & 0xff);
  x[1] = 0xef;
  x[2] = (uint8)((k >> 8) & 0xff);
  x[3] = (uint8)(0xf0 | ((k >> 16) & 0x0f));
}

/* adds GOTO (vector + offset) at each vector; data already there is replaced */
static int
cmd_vectors(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  unsigned long vec[16], v;
  size_t nvec = 0, pos = 0, run, i, n;
  uint32 lo, hi, a, b;
  uint16 hi16 = 0;
  const char* s;
  buffer out, *o;
  int opened, replaced = 0;
  uint8 *image, *map;

  if(!files)
    return 1;

  if(argc - first != 1) {
    errmsg_warn("vectors takes one hex file", 0);
    return 1;
  }

  if(!str_equal(opt_family, "pic18")) {
    errmsg_warn("unsupported family ", opt_family, 0);
    return 1;
  }

  if(!have_addr || opt_addr == 0) {
    errmsg_warn("vectors needs the application offset, -a ADDR", 0);
    return 1;
  }

  for(s = opt_vectors; *s;) {
    char tok[24];

    n = str_chr(s, ',');
    if(n == 0 || n >= sizeof(tok) || nvec == 16) {
      errmsg_warn("invalid vector list ", opt_vectors, 0);
      return 1;
    }
    byte_copy(tok, n, s);
    tok[n] = '\0';
    if(!number(tok, &v)) {
      errmsg_warn("invalid vector ", tok, 0);
      return 1;
    }
    if(v & 1) {
      errmsg_warn("vector ", tok, " is not word aligned", 0);
      return 1;
    }
    if(v + 4 > opt_addr) {
      errmsg_warn("vector ", tok, " lies in the application (at or above -a)", 0);
      return 1;
    }
    if((v + opt_addr) & 1) {
      errmsg_warn("the target of vector ", tok, " is not word aligned", 0);
      return 1;
    }
    vec[nvec++] = v;
    s += n;
    if(*s == ',')
      s++;
  }

  if(!ihex_range(&files[0], &a, &b)) {
    errmsg_warn("no data records", 0);
    return 1;
  }

  lo = 0;
  hi = b;
  image = alloc(hi);
  map = alloc_zero(hi);
  if(!image || !map) {
    errmsg_warn("out of memory", 0);
    return 1;
  }
  byte_fill(image, hi, (unsigned char)opt_fill);
  ihex_copy_to(&files[0], lo, image, map, hi);

  for(i = 0; i < nvec; i++) {
    size_t k;

    for(k = 0; k < 4; k++)
      if(map[vec[i] + k])
        replaced++;

    pic18_goto(image + vec[i], vec[i] + opt_addr);
    byte_fill(map + vec[i], 4, 1);
  }

  if(replaced)
    errmsg_warn("replaced existing data at a vector", 0);

  if((opened = output_open(&out)) < 0)
    return 1;

  o = output_buffer(&out, opened);

  {
    flat f;

    f.image = image;
    f.map = map;
    f.lo = 0;
    f.size = hi;

    while((run = flat_run(&f, &pos))) {
      ihex_write_data(o, &hi16, (uint32)pos, image + pos, run, (unsigned)opt_width);
      pos += run;
    }
  }

  ihex_write_eof(o);
  output_close(&out, opened);
  return 0;
}

static int
cmd_sum(int argc, char* argv[], int first) {
  ihex_file* files = load_all(argc, argv, first);
  size_t i, bytes = 0;
  unsigned long sum = 0;
  flat f;

  if(!files || flat_open(&f, files, argc - first) < 0)
    return 1;

  for(i = 0; i < f.size; i++)
    if(f.map[i]) {
      bytes++;
      sum += f.image[i];
    }

  buffer_puts(buffer_1, "bytes   ");
  buffer_putulong(buffer_1, bytes);
  buffer_puts(buffer_1, "\nsum16   0x");
  buffer_putxlong0u(buffer_1, sum & 0xffffu, 4);
  buffer_puts(buffer_1, "\nsum32   0x");
  buffer_putxlong0u(buffer_1, sum & 0xffffffffu, 8);
  buffer_puts(buffer_1, "\nchk8    0x");
  buffer_putxlong0u(buffer_1, (0x100u - (sum & 0xffu)) & 0xffu, 2);
  buffer_putnlflush(buffer_1);
  return 0;
}

int
main(int argc, char* argv[]) {
  const char* cmd;
  int first;

  prog = argv[0];
  errmsg_iam(argv[0]);

  if(argc < 2 || str_equal(argv[1], "-h") || str_equal(argv[1], "--help")) {
    usage(argv[0]);
    return argc < 2 ? 2 : 0;
  }

  cmd = argv[1];
  first = options(argc - 1, argv + 1);

  if(first == -2)
    return 0;
  if(first < 0)
    return 2;

  first += 1;

  if(str_equal(cmd, "info"))
    return cmd_info(argc, argv, first);
  if(str_equal(cmd, "records"))
    return cmd_records(argc, argv, first);
  if(str_equal(cmd, "dump"))
    return cmd_dump(argc, argv, first);
  if(str_equal(cmd, "merge") || str_equal(cmd, "cut") || str_equal(cmd, "extract"))
    return cmd_merge(argc, argv, first);
  if(str_equal(cmd, "bin"))
    return cmd_bin(argc, argv, first);
  if(str_equal(cmd, "frombin"))
    return cmd_frombin(argc, argv, first);
  if(str_equal(cmd, "verify"))
    return cmd_verify(argc, argv, first);
  if(str_equal(cmd, "vectors"))
    return cmd_vectors(argc, argv, first);
  if(str_equal(cmd, "sum"))
    return cmd_sum(argc, argv, first);

  errmsg_warn("unknown command ", cmd, 0);
  usage(argv[0]);
  return 2;
}
