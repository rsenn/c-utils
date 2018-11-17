#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/uint64.h"
#include "lib/uint8.h"
#include "lib/errmsg.h"
#include "lib/stralloc.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/str.h"

#if WINDOWS
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
  array records;
} patch_t;

static array patches;

extern int buffer_dummyreadmmap();

void
buffer_filename(buffer* b, stralloc* sa) {
  stralloc_zero(sa);
  stralloc_nul(sa);

#ifdef F_GETPATH
  stralloc_ready(sa, MAXPATHLEN + 1);

  fcntl(b->fd, F_GETPATH, sa->s);
  stralloc(b->fd, F_GETPATH, sa->s);
  stralloc_shrink(sa);
#else
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
  if(CopyFileA(orig.s, backup.s, FALSE) != TRUE) return -1;

  if((fd = open_rw(orig.s)) == -1) return -1;
#else
  if(rename(orig.s, backup.s) ==  -1) return -1;

  if((fd = open_rw(orig.s)) == -1) return -1;

  if(io_sendfile(fd, b->fd, 0, size) == -1) return -1;
#endif

  buffer_close(b);

  if(buffer_mmapshared_fd(b, fd)) return -1;

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

void
new_patch(const char* name, size_t file_size) {
  patch_t* patch = array_allocate(&patches, sizeof(patch_t), array_length(&patches, sizeof(patch_t)));

  patch->name = name;
  patch->file_size = file_size;
}

void
patch(unsigned char* p, size_t i, unsigned char from, unsigned char to) {
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

  if(p->file_size && p->file_size != n) return -1;

  for(i = 0; i < nrec; ++i) {
    record_t* r = array_get(&p->records, sizeof(record_t), i);

    if(x[r->offset] != r->from) {
      if(x[r->offset] == r->to)
        ++done;
      else
        return -1;
    }
  }

  if(done == nrec) return 1;
  return 0;
}

patch_t*
patch_find(unsigned char* x, size_t n) {
  size_t i, np = array_length(&patches, sizeof(patch_t));
  for(i = 0; i < np; ++i) {
    patch_t* p = array_get(&patches, sizeof(patch_t), i);

    if(patch_check(x, n, p) >= 0) return p;
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

  if(p->file_size && p->file_size != n) return;

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
                       "\n", 0);
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
  //x = (unsigned char*)mmap_shared(argv[index], &n);

  while(++index < argc) {
    uint64 addr = 0;
    uint64 val_cmp = 0, val_set = 0;
    // size_t s_cmp = 0, s_set = 0;

    char* spec = argv[index];
    char sym = spec[0], *s = &spec[1];

    if(sym == '@') {
      scan_xint64(s, &addr);
    } else if(sym == '?') {
    } else if(sym == '=') {
      // s_set = (scan_xint64(s, &val_set) + 1) / 2;
    } else {
      buffer_putm_3(buffer_2, "ERROR: ", spec, "\n");
      buffer_putnlflush(buffer_2);
      return 2;
    }
  }

  /* Sublime Text 3176 */

  /* Linux x32 */
  new_patch("Sublime Text 3176 Linux x86", 0);
  patch(x, 0xD779, 0x00, 0x01);
  patch(x, 0xC068, 0x38, 0x08);
  patch(x, 0xC069, 0x00, 0x01);
  patch(x, 0x482C5, 0x83, 0xC3);
  patch(x, 0x482C6, 0xEC, 0x90);
  patch(x, 0x482C7, 0x0C, 0x90);

  /* Linux x64 */
  new_patch("Sublime Text 3176 Linux x64", 0);
  patch(x, 0xeb83, 0x00, 0x01); /* Persistent License Check */
  patch(x, 0xd539, 0x00, 0x01); /* Initial License Check */
  patch(x, 0xd538, 0x38, 0x08);
  patch(x, 0x460b5, 0x53, 0xC3); /* Software Update Prompt */

  /* Windows x86 */
  new_patch("Sublime Text 3176 Windows x86", 0);
  patch(x, 0xeb83, 0x00, 0x01); /* Persistent License Check */
  patch(x, 0x267CA, 0x00, 0x01);
  patch(x, 0x26C4F, 0x38, 0x08);
  patch(x, 0x26C50, 0x00, 0x01);
  patch(x, 0x50AFA, 0x55, 0xC3);

  /* Windows x64 */
  new_patch("Sublime Text 3176 Windows x64", 0);
  patch(x, 0x3985A, 0x00, 0x01);
  patch(x, 0x3A073, 0x38, 0x08);
  patch(x, 0x3A074, 0x00, 0x01);
  patch(x, 0x792FB, 0x57, 0xC3);

  patch(x, 0x3985A, 0x00, 0x01); /* Persistent License Check */
  patch(x, 0x3A073, 0x38, 0x08);
  patch(x, 0x3A074, 0x00, 0x01); /* Initial License Check */
  patch(x, 0x792FB, 0x57, 0xC3); /* Software Update Prompt */

  /* eagle-lin32-7.2.0 */
  new_patch("EAGLE 7.2.0 Linux x86", 20629928);
  patch(x, 0x00251874, 0x0f, 0x31);
  patch(x, 0x00251875, 0xb6, 0xc0);
  patch(x, 0x00251876, 0x46, 0x90);
  patch(x, 0x00251877, 0x06, 0x90);
  patch(x, 0x00251878, 0xc1, 0x90);
  patch(x, 0x00251879, 0xe8, 0x90);
  patch(x, 0x0025187a, 0x04, 0x90);
  patch(x, 0x0044bbb4, 0x74, 0xeb);

  /* eagle-lin32-7.7.0 */
  new_patch("EAGLE 7.7.0 Linux x86", 27784220);
  patch(x, 0x002ec33e, 0x0f, 0x31);
  patch(x, 0x002ec33f, 0xb6, 0xc0);
  patch(x, 0x002ec340, 0x46, 0x90);
  patch(x, 0x002ec341, 0x06, 0x90);
  patch(x, 0x002ec342, 0xc1, 0x90);
  patch(x, 0x002ec343, 0xe8, 0x90);
  patch(x, 0x002ec344, 0x04, 0x90);
  patch(x, 0x0051fc14, 0x74, 0xeb);

  if((p = patch_find(x, n))) {
    int check = patch_check(x, n, p);

    buffer_puts(buffer_2,  check == 1 ? "Already patched: " : "Found patch: ");
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
