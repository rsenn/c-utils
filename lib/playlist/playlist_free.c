#include "../playlist_internal.h"
#include "../alloc.h"

/* releases what the readers allocated; pl can be used for another playlist */
void
playlist_free(playlist* pl) {
  struct playlist_state* st = pl->ptr;
  size_t i;

  if(!st)
    return;
  if(pl->type == M3U || pl->type == PLS || pl->type == XSPF) {
    stralloc_free(&st->entry.title);
    stralloc_free(&st->entry.path);
    stralloc_free(&st->tag);
    for(i = 0; i < st->listlen; ++i) {
      stralloc_free(&st->list[i].title);
      stralloc_free(&st->list[i].path);
    }
    if(st->list)
      alloc_free(st->list);
    alloc_free(st);
  }
  pl->ptr = NULL;
}
