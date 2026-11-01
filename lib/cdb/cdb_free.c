#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../byte.h"
#include "../cdb.h"
#include "../havepread.h"
#ifdef __MINGW32__
#include "../windows.h"
#else
#include <sys/mman.h>
#endif

void cdb_free(struct cdb *c) {
  if (c->map) {
#ifdef __MINGW32__
    UnmapViewOfFile(c->map);
#else
    munmap(c->map,c->size);
#endif
    c->map = 0;
  }
}
