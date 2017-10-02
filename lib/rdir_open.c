#include <assert.h>
#include "rdir.h"

int rdir_open(struct rdir_s* d, const char* p) {
  int ret = dir_open(&d->dir, p);
  d->prev = NULL;
  return ret;
}
