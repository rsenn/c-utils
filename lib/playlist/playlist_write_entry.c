#include "playlist.h"

int
playlist_write_entry(buffer* b, playlist* pl, playlist_entry* e) {
  ++pl->count;
  switch(pl->type) {
  case M3U: {
    buffer_puts(b, "#EXTINF:");
    buffer_putulong(b, e->length);
    buffer_puts(b, ",");
    buffer_putsa(b, &e->title);
    buffer_puts(b, "\n");
    buffer_putsa(b, &e->path);
    buffer_puts(b, "\n");
    break;
  }
  case PLS: {
    buffer_puts(b, "File");
    buffer_putulong(b, pl->count);
    buffer_puts(b, "=");
    buffer_putsa(b, &e->path);
    buffer_puts(b, "\nTitle");
    buffer_putulong(b, pl->count);
    buffer_puts(b, "=");
    buffer_putsa(b, &e->title);
    buffer_puts(b, "\nLength");
    buffer_putulong(b, pl->count);
    buffer_puts(b, "=");
    buffer_putulong(b, e->length);
    buffer_puts(b, "\n");
    break;
  }
  case XSPF: {

    buffer_puts(b, "<track>\n<location>");
    buffer_putsa(b, &e->path);
    buffer_puts(b, "</location>\n<title>");
    buffer_putsa(b, &e->title);
    buffer_puts(b, "</title>\n<duration>");
    buffer_putulong(b, e->length);
    buffer_puts(b, "</duration></track>\n");
    break;
  }
  case UNKNOWN:
  default: {
    return 0;
  }
  }
  return 1;
}

