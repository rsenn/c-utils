#include "../playlist.h"
#include "../str.h"
#include "../byte.h"

int
playlist_write_entry(buffer* b, playlist* pl, playlist_entry* e) {
  stralloc* title =  e->title.len ? &e->title : &e->path;
  ++pl->count;
  switch(pl->type) {
    case M3U: {
      buffer_puts(b, "#EXTINF:");
      buffer_putulong(b, e->length);
      buffer_puts(b, ",");
      buffer_putsa(b, title);
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
      buffer_putsa(b, title);
      buffer_puts(b, "\nLength");
      buffer_putulong(b, pl->count);
      buffer_puts(b, "=");
      buffer_putulong(b, e->length);
      buffer_puts(b, "\n");
      break;
    }
    case XSPF: {
      size_t i = byte_chr(e->path.s, e->path.len, ':');

      buffer_puts(b, "    <track>\n      <location>");

      if(str_diffn(&e->path.s[i], "://", 3))
        buffer_puts(b, "file://");

      buffer_putsa(b, &e->path);
      buffer_puts(b, "</location>\n      <title>");
      buffer_putsa(b, title);
      buffer_puts(b, "</title>\n      <duration>");
      buffer_putulong(b, e->length);
      buffer_puts(b, "</duration>\n    </track>\n");
      break;
    }
    case UNKNOWN:
    default: {
      return 0;
    }
  }
  return 1;
}

