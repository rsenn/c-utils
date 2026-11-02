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

int cdb_find(struct cdb *c,const unsigned char *key,unsigned long int len) {
  cdb_findstart(c);
  return cdb_findnext(c,key,len);
}
