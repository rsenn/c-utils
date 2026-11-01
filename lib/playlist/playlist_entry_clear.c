#include "../playlist_internal.h"

/* empties title and path and forgets the length, so that the next entry does not
 * inherit anything from this one */
void
playlist_entry_clear(playlist_entry* e) {
  stralloc_zero(&e->title);
  stralloc_zero(&e->path);
  e->length = 0;
}
