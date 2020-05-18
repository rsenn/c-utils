#include "../rdir.h"
#include "../byte.h"

int
rdir_open(rdir_t* d, const char* p) {
  int ret;
  byte_zero(d, sizeof(rdir_t));
  ret = dir_open(&d->dir, p);
  stralloc_init(&d->sa);
  stralloc_copys(&d->sa, p);
  return ret;
}
