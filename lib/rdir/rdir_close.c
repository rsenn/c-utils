#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../dir.h"
#include "../rdir.h"

void
rdir_close(rdir_t* d) {
  dir_close(&d->dir);
  /* free(d->rdir_int); */
}
