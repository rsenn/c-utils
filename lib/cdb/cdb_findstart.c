#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../byte.h"
#include "../cdb.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef __MINGW32__
#include "../windows.h"
#else
#include <sys/mman.h>
#endif

void cdb_findstart(struct cdb *c) {
  c->loop = 0;
}
