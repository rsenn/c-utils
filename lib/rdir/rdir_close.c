
#include "../dir.h"
#include "../rdir.h"

void
rdir_close(rdir_t* d) {
  rdir_t* p;
  stralloc_free(&d->sa);

  for(p = d; p; p = p->prev) dir_close(&p->dir);

  p = d;
  for(p = d->prev; p; p = d) {
    d = p->prev;
    free(p);
  }
}
