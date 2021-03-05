#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/uint64.h"
#include "lib/uint8.h"
#include "lib/errmsg.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"

#if WINDOWS_NATIVE
#include <windows.h>
#endif

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>

char hexdigits[] = "0123456789abcdef";

typedef struct {
  uint64 offset;
  uint8 from;
  uint8 to;
} record_t;

typedef struct {
  const char* name;
  size_t file_size;
  uint32 crc32;
  array records;
} patch_t;

static array patches;

extern ssize_t buffer_dummyreadmmap();

uint32
crc32(uint32 crc, const char* data, size_t size) {
  uint32 i, r = ~0;
  const char* end = data + size;

  while(data < end) {
    r ^= (unsigned char)*data++;

    for(i = 0; i < 8; i++) {
      uint32 t = ~((r & 1) - 1);
      r = (r >> 1) ^ (0xedb88320 & t);
    }
  }

  return ~r;
}

int
get_crc32(const char* filename, uint32* crc) {
  size_t n;
  const char* x;

  if((x = mmap_read(filename, &n)) == 0)
    return -1;

  *crc = crc32(0, x, n);

  mmap_unmap(x, n);
  return 0;
}

uint32
byte_crc32(const char* x, size_t n) {
  uint32 r;
  r = crc32(0, (const char*)x, n);
  return r;
}

uint32
buffer_crc32(buffer* b) {
  return byte_crc32(b->x, b->n);
}

void
buffer_filename(buffer* b, stralloc* sa) {
  stralloc_zero(sa);
  stralloc_nul(sa);

#ifdef F_GETPATH
  stralloc_ready(sa, MAXPATHLEN + 1);

  fcntl(b->fd, F_GETPATH, sa->s);
  stralloc(b->fd, F_GETPATH, sa->s);
  stralloc_shrink(sa);
#elif !WINDOWS_NATIVE
  {
    stralloc procp;
    stralloc_init(&procp);
    stralloc_cats(&procp, "/proc/");
    stralloc_catulong(&procp, getpid());
    stralloc_cats(&procp, "/fd/");
    stralloc_catulong(&procp, b->fd);
    stralloc_nul(&procp);
    path_readlink(procp.s, sa);
  }
#else
  mmap_filename(b->x, sa);
#endif
}

int
buffer_backup(buffer* b) {
  int fd;
  size_t size = b->n;
  stralloc orig, backup;
  stralloc_init(&orig);
  stralloc_init(&backup);

  if(b->op == (buffer_op_proto*)&buffer_dummyreadmmap) {
    buffer_munmap(b);
  }
  buffer_filename(b, &orig);

  stralloc_copy(&backup, &orig);
  stralloc_cats(&backup, ".orig");
  stralloc_nul(&backup);
  stralloc_nul(&orig);

#if WINDOWS_NATIVE
  if(CopyFileA(orig.s, backup.s, FALSE) != TRUE)
    return -1;

  if((fd = open_rw(orig.s)) == -1)
    return -1;
#else
  if(rename(orig.s, backup.s) == -1)
    return -1;

  if((fd = open_rw(orig.s)) == -1)
    return -1;

  if(io_sendfile(fd, b->fd, 0, size) == -1)
    return -1;
#endif

  buffer_close(b);

  if(buffer_mmapshared_fd(b, fd))
    return -1;

  return 0;
}

void
print(unsigned char* p, size_t i, unsigned char from, unsigned char to) {
  buffer_puts(buffer_2, "0x");
  buffer_putxlong(buffer_2, i);
  buffer_putspace(buffer_2);
  buffer_puts(buffer_2, "0x");
  buffer_PUTC(buffer_2, hexdigits[from >> 4]);
  buffer_PUTC(buffer_2, hexdigits[from & 0x0f]);
  buffer_putspace(buffer_2);
  buffer_puts(buffer_2, "0x");
  buffer_PUTC(buffer_2, hexdigits[to >> 4]);
  buffer_PUTC(buffer_2, hexdigits[to & 0x0f]);
}

patch_t*
patch_new(const char* name, size_t file_size, uint32 crc32) {
  patch_t* patch = array_allocate(&patches, sizeof(patch_t), array_length(&patches, sizeof(patch_t)));

  patch->name = name;
  patch->file_size = file_size;
  patch->crc32 = crc32;

  return patch;
}

void
patch(size_t i, unsigned char from, unsigned char to) {
  patch_t* patch = array_get(&patches, sizeof(patch_t), array_length(&patches, sizeof(patch_t)) - 1);
  record_t* rec = array_allocate(&patch->records, sizeof(record_t), array_length(&patch->records, sizeof(record_t)));

  rec->offset = i;
  rec->from = from;
  rec->to = to;
}

int
patch_check(unsigned char* x, size_t n, patch_t* p) {
  size_t i, nrec = array_length(&p->records, sizeof(record_t));
  size_t done = 0;
  uint32 crc;
  if(p->file_size && p->file_size != n)
    return -1;
  crc = byte_crc32((const char*)x, n);
  if(p->crc32 && p->crc32 != crc)
    return -1;

  buffer_putm_internal(buffer_2, "Checking for '", p->name, "'...", 0);
  buffer_flush(buffer_2);

  if(p->file_size) {
    if(p->file_size && p->file_size != n) {
      buffer_putsflush(buffer_2, "SIZE MISMATCH\n");
      return -1;
    }

    buffer_puts(buffer_2, "SIZE ");
    buffer_putulong(buffer_2, p->file_size);
    buffer_puts(buffer_2, " OK");
  }

  buffer_putnlflush(buffer_2);

  for(i = 0; i < nrec; ++i) {
    record_t* r = array_get(&p->records, sizeof(record_t), i);

    buffer_puts(buffer_2, "position 0x");
    buffer_putxlong0(buffer_2, r->offset, 8);
    buffer_puts(buffer_2, " is 0x");
    buffer_putxlong0(buffer_2, x[r->offset] == r->to ? r->to : r->from, 2);
    if(x[r->offset] == r->from || x[r->offset] == r->to) {

      buffer_puts(buffer_2, x[r->offset] == r->from ? " OK" : " ALREADY PATCHED");
      buffer_putnlflush(buffer_2);
    } else {
      buffer_puts(buffer_2, " NO (0x");
      buffer_putxlong0(buffer_2, x[r->offset], 2);
      buffer_putsflush(buffer_2, ")\n");
      return -2;
    }

    if(x[r->offset] == r->to)
      ++done;
  }

  if(done == nrec)
    return 1;
  return 0;
}

patch_t*
patch_find(unsigned char* x, size_t n) {
  size_t i, np = array_length(&patches, sizeof(patch_t));
  for(i = 0; i < np; ++i) {
    patch_t* p = array_get(&patches, sizeof(patch_t), i);

    if(patch_check(x, n, p) >= 0)
      return p;
  }
  return NULL;
}

void
record_apply(unsigned char* p, size_t i, unsigned char from, unsigned char to) {
  buffer_puts(buffer_2, "patch ");
  print(p, i, from, to);
  if(p[i] == from) {
    p[i] = to;
    buffer_puts(buffer_2, " ok");
  } else {
    buffer_puts(buffer_2, " fail");
  }
  buffer_putnlflush(buffer_2);
}

void
patch_apply(unsigned char* x, size_t n, patch_t* p) {
  size_t i, nrec = array_length(&p->records, sizeof(record_t));

  if(p->file_size && p->file_size != n)
    return;

  for(i = 0; i < nrec; ++i) {
    record_t* r = array_get(&p->records, sizeof(record_t), i);

    record_apply(x, r->offset, r->from, r->to);
  }
}

void
usage(const char* av0) {
  buffer_putm_internal(buffer_2,
                       "Usage: ",
                       av0,
                       " <file> [edit-specifier]\n"
                       "\n"
                       "  [edit-specifier] is "
                       "<address>=<value>\n"
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_2);
}

int
main(int argc, char* argv[]) {
  int ret = 1;
  int index = 1;
  patch_t* p;
  size_t n;
  unsigned char* x;
  buffer file;
  int fd;
  uint64 offset = 0;

  errmsg_iam(argv[0]);

  if(argc <= index) {
    usage(argv[0]);
    return 1;
  }

  if((fd = open_read(argv[index])) == -1 || buffer_mmapread_fd(&file, fd)) {
    errmsg_warnsys(str_basename(argv[0]), ": opening file: ", argv[index], 0);
    return 1;
  }

  x = (unsigned char*)file.x;
  n = file.n;
  // x = (unsigned
  // char*)mmap_shared(argv[index], &n);

  if(index + 1 < argc) {
    patch_new("command line", file.n, 0);

    while(++index < argc) {
      uint64 addr = 0;
      uint64 val_cmp = 0, val_set = 0;
      // size_t s_cmp = 0, s_set = 0;

      char* spec = argv[index];
      char sym = spec[0], *s = &spec[1];

      while((sym = *spec++)) {
        size_t n = 0;
        if(sym == '@' && isdigit(*spec)) {
          n = scan_xlonglong(spec, &addr);
          spec += n;
          continue;
        }
        if(sym == '\0')
          break;
        if(sym == '?') {
        }
        if((sym == '+' || sym == '*') && isdigit(*spec)) {
          int64 num;
          n = scan_xlonglong(spec, &num);
          if(n > 0) {
            if(sym == '+')
              offset += num;
            if(sym == '*')
              offset *= num;
          }
          spec += n;
          continue;
        }
        if(sym == '=') {
          uint8 ch = 0;
          do {
            n = scan_xchar(spec, &ch);
            if(n >= 1) {
              patch(offset + addr, file.x[offset + addr], ch);
            }

            addr++;
            spec += n;

          } while(*spec);

        } /*else {
          buffer_putm_internal(buffer_2,
        "ERROR: ", spec, "\n", 0);
          buffer_putnlflush(buffer_2);
          return 2;
        }*/
        if(n == 0)
          break;
        spec += n;
      }
    }
  }

  patch_new("Sublime Text 3095 Linux x64", 5402336, 0xa1431014);
  patch(0xd703, 0x85, 0x39);
  patch_new("Sublime Text 3095 Linux x86", 5186596, 0x206b4a7e);
  patch(0xd152, 0x85, 0x39);

  patch_new("Sublime Text 3095 Windows x64", 6152960, 0x47994765);

  patch(0xf0bdf, 0x85, 0x3B);
  patch_new("Sublime Text 3095 Windows x86", 4338432, 0xe1cbd5aa);
  patch(0xa96f4, 0x1a, 0x2a);
  patch_new("Sublime Text 3095 Mac", 12522320, 0x81c9200c);

  patch(0x6615, 0x85, 0x39);

  /* Linux x64 */
  patch_new("Sublime Text 3065 Linux x64", 5173248, 0xa5984d0d);
  patch(0x001BD110, 0x8A, 0x90);
  patch(0x001BD111, 0x9B, 0xB3);
  patch(0x001BD112, 0xB8, 0x01);
  patch(0x001BD113, 0x00, 0x90);
  patch(0x001BD114, 0x00, 0x90);
  patch(0x001BD115, 0x00, 0x90);

  /* Linux x64 */
  patch_new("Sublime Text 3126 Linux x64", 5200392, 0x53b8b50c);
  patch(0xc62e, 0x94, 0x95); /* License Check */

  /* Linux x86 */
  patch_new("Sublime Text 3126 Linux x86", 4992708, 0xf2562728);
  patch(0xC35C, 0x94, 0x95); /* License Check */

  /* Linux x32 */
  patch_new("Sublime Text 3176 Linux x86", 7555888, 0xa7b1b28c);
  patch(0xD779, 0x00, 0x01);
  patch(0xC068, 0x38, 0x08);
  patch(0xC069, 0x00, 0x01);
  patch(0x482C5, 0x83, 0xC3);
  patch(0x482C6, 0xEC, 0x90);
  patch(0x482C7, 0x0C, 0x90);

  /* Linux x64 */
  patch_new("Sublime Text 3176 Linux x64", 7235496, 0x0b9ff058);
  patch(0xeb83, 0x00, 0x01); /* Persistent License
                                Check */

  patch(0xd538, 0x38, 0x08);
  patch(0xd539, 0x00, 0x01);  /* Initial License Check */
  patch(0x460b5, 0x53, 0xC3); /* Software Update Prompt
                               */

  /* Windows x86 */
  patch_new("Sublime Text 3176 Windows x86", 5186160, 0xd4cff4dd);
  patch(0xeb83, 0x00, 0x01); /* Persistent License
                                Check */
  patch(0x267CA, 0x00, 0x01);
  patch(0x26C4F, 0x38, 0x08);
  patch(0x26C50, 0x00, 0x01);
  patch(0x50AFA, 0x55, 0xC3);

  /* Windows x64 */
  patch_new("Sublime Text 3176 Windows x64", 7352944, 0);
  patch(0x3985A, 0x00, 0x01); /* Persistent License
                                 Check */
  patch(0x3A073, 0x38, 0x08);
  patch(0x3A074, 0x00, 0x01); /* Initial License Check */
  patch(0x792FB, 0x57, 0xC3); /* Software Update Prompt
                               */

  /* Windows x64 */
  patch_new("Sublime Text 3193 Windows x64", 8330872, 0x879cf71d);

  patch(0x58BA04, 0x97, 0x00);
  patch(0x58BA05, 0x94, 0x00);
  patch(0x58BA06, 0x0d, 0x00);

  /* Linux x64 */
  patch_new("Sublime Text 3200 Linux x64", 8766968, 0x94a745c1);

  patch(0x3BEB98, 0x84, 0x85); /* Initial License Check */
  patch(0x477C6E, 0x75, 0x74); /* Persistent License
                                  Check */

  /* Windows x64 */
  patch_new("Sublime Text 3200 Windows x64", 8331384, 0x8261a833);

  patch(0x8545, 0x84, 0x85);   /* Initial License Check */
  patch(0x192487, 0x75, 0x74); /* Persistent License
                                  Check */
  patch(0x8F6A8, 0x48, 0xC3);  /* Purchase License Nag */

  /* Linux x64 */
  patch_new("Sublime Text 3207 Linux x64", 8787520, 0xdafb1051);
  patch(0x3c03dc, 0x84, 0x85); /* Initial License Check */
  patch(0x4797a0, 0x75, 0x74); /* Persistent License
                                  Check */
  patch(0x31d180, 0x75, 0x74); /* Purchase License Nag */

  /* Windows x64 */
  patch_new("Sublime Text 3207 Windows x64", 8348280, 0xd729b18a);

  patch(0x8545, 0x84, 0x85);   /* Initial License Check */
  patch(0x193263, 0x75, 0x74); /* Persistent License
                                  Check */
  patch(0x90315, 0x48, 0xc3);  /* Purchase License Nag */
  patch(0xd6b6c, 0x84, 0x85);  /* Initial Update Check */
  patch(0x4d745a, 0x85, 0x84); /* Menu Update Check */

  /* Linux x86 */
  patch_new("Sublime Text 3211 Linux x86", 9873136, 0x0fd9ad24);
  patch(0x39c5ea, 0x55, 0xb8);
  patch(0x39c5eb, 0x53, 0x01);
  patch(0x39c5ec, 0x57, 0x00);
  patch(0x39c5ed, 0x56, 0x00);
  patch(0x39c5ee, 0x81, 0x00);
  patch(0x39c5ef, 0xec, 0xc3);

  /* Linux x64 */
  patch_new("Sublime Text 3211 Linux x64", 8803928, 0xcc65df3b);
  patch(0x31dbd5, 0x55, 0xb8);
  patch(0x31dbd6, 0x41, 0x01);
  patch(0x31dbd7, 0x57, 0x00);
  patch(0x31dbd8, 0x41, 0x00);
  patch(0x31dbd9, 0x56, 0x00);
  patch(0x31dbda, 0x41, 0xc3);

  /* Windows x86 */
  patch_new("Sublime Text 3211 Windows x86", 6896528, 0xd20af2da);
#if 0 // pre-regged variant
  patch(0x0cdc7, 0x00, 0x10);
  patch(0x7a48f, 0xf2, 0x66);
  patch(0x7a490, 0x0f, 0x83);
  patch(0x7a491, 0x11, 0x09);
  patch(0x7a492, 0x01, 0x10);
  patch(0x7c40e, 0x25, 0x0d);
  patch(0x7c413, 0x00, 0x10);
#endif

  patch(0x7a75e, 0x55, 0xb8);
  patch(0x7a75f, 0x89, 0x01);
  patch(0x7a760, 0xe5, 0x00);
  patch(0x7a761, 0x53, 0x00);
  patch(0x7a762, 0x57, 0x00);
  patch(0x7a763, 0x56, 0xc3);
  /* Windows x64 */
  patch_new("Sublime Text 3211 Windows x64", 8368016, 0xd42a5d56);
#if 0 // pre-regged variant
  patch(0x0e12a, 0x00, 0x10);
  patch(0x8f099, 0x48, 0x80);
  patch(0x8f09a, 0x89, 0x09);
  patch(0x8f09b, 0x01, 0x10);
  patch(0x915aa, 0x25, 0x0d);
  patch(0x915af, 0x00, 0x10);
#endif
  patch(0x8f4b0, 0x55, 0xb8);
  patch(0x8f4b1, 0x41, 0x01);
  patch(0x8f4b2, 0x57, 0x00);
  patch(0x8f4b3, 0x41, 0x00);
  patch(0x8f4b4, 0x56, 0x00);
  patch(0x8f4b5, 0x41, 0xc3);

  /* MacOSX x64 */
  patch_new("Sublime Text 3211 MacOSX x64", 14126480, 0xd76d794a);

  patch(0xda4cf, 0x55, 0xb8);
  patch(0xda4d0, 0x48, 0x01);
  patch(0xda4d1, 0x89, 0x00);
  patch(0xda4d2, 0xe5, 0x00);
  patch(0xda4d3, 0x41, 0x00);
  patch(0xda4d4, 0x57, 0xc3);

  patch_new("Sublime Text 4086 Linux x64", 8525992, 0x5862072d);
  patch(0x352F69, 0x00, 0x1);
  patch(0x4142E8, 0x53, 0xc3);
  patch(0x352242, 0x38, 0x08);
  patch(0x352243, 0x00, 0x01);

  /* eagle-lin32-7.2.0 */
  patch_new("EAGLE 7.2.0 Linux x86", 20629928, 0);
  patch(0x0000854f, 0x2e, 0x00);
  patch(0x00008a10, 0x2e, 0x00);
  patch(0x00251874, 0x0f, 0x31);
  patch(0x00251875, 0xb6, 0xc0);
  patch(0x00251876, 0x46, 0x90);
  patch(0x00251877, 0x06, 0x90);
  patch(0x00251878, 0xc1, 0x90);
  patch(0x00251879, 0xe8, 0x90);
  patch(0x0025187a, 0x04, 0x90);
  patch(0x0044bbb4, 0x74, 0xeb);
  /*
    patch(0x00251874, 0x0f, 0x31);32
    patch(0x00251875, 0xb6, 0xc0);
    patch(0x00251876, 0x46, 0x90);
    patch(0x00251877, 0x06, 0x90);
    patch(0x00251878, 0xc1, 0x90);
    patch(0x00251879, 0xe8, 0x90);
    patch(0x0025187a, 0x04, 0x90);
    patch(0x0044bbb4, 0x74, 0xeb);
  */
  /* eagle-lin32-7.5.0 */
  patch_new("EAGLE 7.5.0 Linux x86", 25715928, 0);
  patch(0x0010cc56, 0xc3, 0x41);
  patch(0x002a4dda, 0x31, 0xc1);
  patch(0x002a4ddb, 0xc0, 0xe8);
  patch(0x002a4ddc, 0x90, 0x04);

  /* eagle-lin32-7.7.0 */
  patch_new("EAGLE 7.7.0 Linux x86", 27784220, 0);

  patch(0x00016a1b, 0x2e, 0x00);
  patch(0x00016a1c, 0x30, 0x00);
  patch(0x00016a1d, 0x2e, 0x00);
  patch(0x00016a1e, 0x30, 0x00);
  patch(0x0001713d, 0x2e, 0x00);
  patch(0x0001713e, 0x30, 0x00);
  patch(0x0001713f, 0x2e, 0x00);
  patch(0x00017140, 0x30, 0x00);
  patch(0x002ec33e, 0x0f, 0x31);
  patch(0x002ec33f, 0xb6, 0xc0);
  patch(0x002ec340, 0x46, 0x90);
  patch(0x002ec341, 0x06, 0x90);
  patch(0x002ec342, 0xc1, 0x90);
  patch(0x002ec343, 0xe8, 0x90);
  patch(0x002ec344, 0x04, 0x90);
  patch(0x0051fc14, 0x74, 0xeb);

  /* eagle-win64-7.6.0 */
  patch_new("EAGLE 7.6.0 Windows x64", 28329984, 0);

  patch(0x000d6932, 0x48, 0x90);
  patch(0x000d6933, 0x8b, 0x90);
  patch(0x000d6934, 0xcf, 0x90);
  patch(0x000d6935, 0xe8, 0x90);
  patch(0x000d6936, 0x96, 0x90);
  patch(0x000d6937, 0x71, 0x90);
  patch(0x000d6938, 0x50, 0x90);
  patch(0x000d6939, 0x00, 0x90);
  patch(0x00244e0c, 0x0f, 0x33);
  patch(0x00244e0d, 0xb6, 0xd2);
  patch(0x00244e0e, 0x56, 0x90);
  patch(0x00244e0f, 0x06, 0x90);
  patch(0x002450b8, 0x75, 0xeb);
  patch(0x003f325d, 0x74, 0xeb);
  patch(0x003f3271, 0x74, 0xeb);

  /* eagle-lin64-7.6.0 */
  patch_new("EAGLE 7.6.0 Linux x64", 25715928, 0);
  patch(0x0010cc56, 0xc3, 0x41);
  patch(0x002a4dda, 0x31, 0xc1);
  patch(0x002a4ddb, 0xc0, 0xe8);
  patch(0x002a4ddc, 0x90, 0x04);

  /* eagle-win32-7.7.0 */
  patch_new("EAGLE 7.7.0 Windows x86", 21286400, 0);

  patch(0x0017fa6f, 0x06, 0x05);
  patch(0x0017fd4c, 0x75, 0xeb);
  patch(0x002a3063, 0x74, 0xeb);
  patch(0x002a3076, 0x74, 0xeb);
  patch(0x00f51a5e, 0x45, 0x55);
  patch(0x00f51a5f, 0x78, 0x6c);
  patch(0x00f51a60, 0x70, 0x74);
  patch(0x00f51a61, 0x72, 0x69);
  patch(0x00f51a62, 0x65, 0x6d);
  patch(0x00f51a63, 0x73, 0x61);
  patch(0x00f51a64, 0x73, 0x74);
  patch(0x00f51a65, 0x00, 0x65);
  patch(0x00f53199, 0x74, 0x53);
  patch(0x00f5319a, 0x6f, 0x41);
  patch(0x00f5319b, 0x20, 0x4b);
  patch(0x00f5319c, 0x72, 0x49);
  patch(0x00f5319d, 0x75, 0x53);
  patch(0x00f5319e, 0x6e, 0x20);
  patch(0x00f5319f, 0x20, 0x55);
  patch(0x00f531a0, 0x45, 0x4b);
  patch(0x00f531a1, 0x41, 0x52);
  patch(0x00f531a2, 0x47, 0x20);
  patch(0x00f531a3, 0x4c, 0x20);
  patch(0x00f531a4, 0x45, 0x20);

  /* eagle-win64-7.7.0 */
  patch_new("EAGLE 7.7.0 Windows x64", 28654080, 0);

  patch(0x000d7800, 0x48, 0x90);
  patch(0x000d7801, 0x8b, 0x90);
  patch(0x000d7802, 0xcf, 0x90);
  patch(0x000d7803, 0xe8, 0x90);
  patch(0x000d7804, 0xd8, 0x90);
  patch(0x000d7805, 0xec, 0x90);
  patch(0x000d7806, 0x50, 0x90);
  patch(0x000d7807, 0x00, 0x90);
  patch(0x00246838, 0x0f, 0x33);
  patch(0x00246839, 0xb6, 0xd2);
  patch(0x0024683a, 0x56, 0x90);
  patch(0x0024683b, 0x06, 0x90);
  patch(0x00246ae4, 0x75, 0xeb);
  patch(0x003f52c9, 0x74, 0xeb);
  patch(0x003f52dd, 0x74, 0xeb);

  if((p = patch_find(x, n))) {
    int check = patch_check(x, n, p);

    buffer_puts(buffer_2, check == 1 ? "Already patched: " : "Found patch: ");
    buffer_puts(buffer_2, p->name);
    buffer_putnlflush(buffer_2);

    if(check == 0) {
      buffer_backup(&file);
      patch_apply((unsigned char*)file.x, file.a, p);
      ret = 0;
    }
  }

  buffer_close(&file);
  return ret;
}
