#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

extern int buffer_dummyreadmmap();

uint32
crc32(uint32 crc, const char* data, size_t size) {
  uint32 i, r = ~0;
  const char* end = data + size;

  while(data < end) {
    r ^= *data++;

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
  char* x;

  if(mmap_read(filename, &n) == 0)
    return -1;

  *crc = crc32(0, (const char*)x, n);

  mmap_unmap(x, n);
  return 0;
}

uint32
buffer_crc32(buffer* b) {
  uint32 r;
  r = crc32(0, (const char*)b->x, b->n);
  return r;
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

  if(p->file_size && p->file_size != n)
    return -1;

  for(i = 0; i < nrec; ++i) {
    record_t* r = array_get(&p->records, sizeof(record_t), i);

    if(x[r->offset] != r->from) {
      if(x[r->offset] == r->to)
        ++done;
      else
        return -1;
    }
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
                       "  [edit-specifier] is <address>=<value>\n"
                       "\n",
                       0);
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
  // x = (unsigned char*)mmap_shared(argv[index], &n);

  while(++index < argc) {
    uint64 addr = 0;
    uint64 val_cmp = 0, val_set = 0;
    // size_t s_cmp = 0, s_set = 0;

    char* spec = argv[index];
    char sym = spec[0], *s = &spec[1];

    if(sym == '@') {
      scan_xlonglong(s, &addr);
    } else if(sym == '?') {
    } else if(sym == '=') {
      // s_set = (scan_xlonglong(s, &val_set) + 1) / 2;
    } else {
      buffer_putm_3(buffer_2, "ERROR: ", spec, "\n");
      buffer_putnlflush(buffer_2);
      return 2;
    }
  }

  /* Linux x64 */
  patch_new("Sublime Text 3065 Linux x64", 13170144, 0);
  patch(0x00005ac2, 0x2e, 0xe2);
  patch(0x001BD111, 0x8A, 0x90);
  patch(0x001BD112, 0x9B, 0xB3);
  patch(0x001BD113, 0xB8, 0x01);
  patch(0x001BD114, 0x00, 0x90);
  patch(0x001BD115, 0x00, 0x90);
  patch(0x001BD116, 0x00, 0x90);

  /* Linux x64 */
  patch_new("Sublime Text 3126 Linux x64", 5200392, 0);
  patch(0xc62e, 0x94, 0x95); /* License Check */

  /* Linux x32 */
  patch_new("Sublime Text 3176 Linux x86", 0, 0);
  patch(0xD779, 0x00, 0x01);
  patch(0xC068, 0x38, 0x08);
  patch(0xC069, 0x00, 0x01);
  patch(0x482C5, 0x83, 0xC3);
  patch(0x482C6, 0xEC, 0x90);
  patch(0x482C7, 0x0C, 0x90);

  /* Linux x64 */
  patch_new("Sublime Text 3176 Linux x64", 0, 0);
  patch(0xeb83, 0x00, 0x01); /* Persistent License Check */
  patch(0xd539, 0x00, 0x01); /* Initial License Check */
  patch(0xd538, 0x38, 0x08);
  patch(0x460b5, 0x53, 0xC3); /* Software Update Prompt */

  /* Windows x86 */
  patch_new("Sublime Text 3176 Windows x86", 0, 0);
  patch(0xeb83, 0x00, 0x01); /* Persistent License Check */
  patch(0x267CA, 0x00, 0x01);
  patch(0x26C4F, 0x38, 0x08);
  patch(0x26C50, 0x00, 0x01);
  patch(0x50AFA, 0x55, 0xC3);

  /* Windows x64 */
  patch_new("Sublime Text 3176 Windows x64", 7352944, 0);
  patch(0x3985A, 0x00, 0x01); /* Persistent License Check */
  patch(0x3A073, 0x38, 0x08);
  patch(0x3A074, 0x00, 0x01); /* Initial License Check */
  patch(0x792FB, 0x57, 0xC3); /* Software Update Prompt */

  /* Windows x64 */
  patch_new("Sublime Text 3193 Windows x64", 0, 0);

  patch(0x58BA04, 0x97, 0x00);
  patch(0x58BA05, 0x94, 0x00);
  patch(0x58BA06, 0x0d, 0x00);

  /* Linux x64 */
  patch_new("Sublime Text 3200 Linux x64", 0, 0);

  patch(0x3BEB98, 0x84, 0x85); /* Initial License Check */
  patch(0x477C6E, 0x75, 0x74); /* Persistent License Check */

  /* Windows x64 */
  patch_new("Sublime Text 3200 Windows x64", 0, 0);

  patch(0x8545, 0x84, 0x85);   /* Initial License Check */
  patch(0x192487, 0x75, 0x74); /* Persistent License Check */
  patch(0x8F6A8, 0x48, 0xC3);  /* Purchase License Nag */

  /* Linux x64 */
  patch_new("Sublime Text 3207 Linux x64", 0, 0);
  patch(0x3c03dc, 0x84, 0x85); /* Initial License Check */
  patch(0x4797a0, 0x75, 0x74); /* Persistent License Check */
  patch(0x31d180, 0x75, 0x74); /* Purchase License Nag */

  /* Windows x64 */
  patch_new("Sublime Text 3207 Windows x64", 0, 0);

  patch(0x8545, 0x84, 0x85);   /* Initial License Check */
  patch(0x193263, 0x75, 0x74); /* Persistent License Check */
  patch(0x90315, 0x48, 0xC3);  /* Purchase License Nag */
  patch(0xD6B6C, 0x84, 0x85);  /* Initial Update Check */
  patch(0x4D745A, 0x85, 0x84); /* Menu Update Check */

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
