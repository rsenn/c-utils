#ifndef PLAYLIST_INTERNAL_H
#define PLAYLIST_INTERNAL_H

#include "playlist.h"
#include "xml.h"

/* what a playlist reader keeps between calls; allocated by playlist_m3u,
 * playlist_pls and playlist_xspf, released by playlist_free */
struct playlist_state {
  buffer* in;
  playlist_entry entry; /* the entry being collected (m3u, xspf) */
  playlist_entry* list; /* entries by track number (pls) */
  size_t listlen;
  int flushed;
  xmlreader reader; /* xspf */
  stralloc tag;
};

struct playlist_state* playlist_state_new(playlist* pl, buffer* b);
void playlist_entry_clear(playlist_entry* e);

#endif
