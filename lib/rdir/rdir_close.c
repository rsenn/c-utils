#include "../dir_internal.h"
#include "../rdir.h"

#include <stdlib.h>

void
rdir_close(rdir_t* rd) {
  rdir_t *d, *prev;
  for(d = rd; d; d = prev) {
    prev = d->prev;
    if(dir_INTERNAL(&d->dir) && dir_INTERNAL(&d->dir)->dir_handle)
      dir_close(&d->dir);
  }
  for(d = rd->prev; d; d = prev) {
    prev = d->prev;
    if(d->sa.s)
      stralloc_free(&d->sa);
    free(d);
  }
}
