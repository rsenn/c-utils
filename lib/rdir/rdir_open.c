#include "../rdir.h"
#include <assert.h>

int
rdir_open(rdir_t* d, const char* p) {
  int ret = dir_open(&d->dir, p);
  stralloc_init(&d->sa);
  stralloc_copys(&d->sa, p);
  d->prev = NULL;
  return ret;
}
