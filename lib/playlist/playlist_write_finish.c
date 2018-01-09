#include "playlist.h"

int
playlist_write_finish(buffer* b, playlist* pl) {
  switch(pl->type) {
    case M3U: {
      break;
    }
    case PLS: {
      buffer_puts(b, "NumberOfEntries=");
      buffer_putulong(b, pl->count);
      break;
    }
    case XSPF: {
      buffer_puts(b, "\n</trackList>\n</playlist>\n");
      break;
    }
    case UNKNOWN: 
    default: {
               return 0;
    }
  }
  return 1;
}

