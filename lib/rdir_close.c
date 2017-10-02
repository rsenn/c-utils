#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rdir.h"
#include "dir.h"

void rdir_close(struct rdir_s* d) {
  dir_close(&d->dir);
  //free(d->rdir_int);
}
