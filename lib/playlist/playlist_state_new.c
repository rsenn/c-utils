#include "../playlist_internal.h"
#include "../alloc.h"
#include "../byte.h"

/* allocates the reader state of pl (a former one is released first) */
struct playlist_state*
playlist_state_new(playlist* pl, buffer* b) {
  struct playlist_state* st;

  playlist_free(pl);
  if(!(st = (struct playlist_state*)alloc_zero(sizeof(struct playlist_state))))
    return NULL;
  st->in = b;
  pl->ptr = st;
  return st;
}
