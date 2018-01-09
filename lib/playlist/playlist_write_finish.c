#include "playlist.h"
#include "byte.h"
#include "fmt.h"
#include <unistd.h>

int
playlist_write_finish(buffer* b, playlist* pl) {
  switch(pl->type) {
    case M3U: {
      break;
    }
    case PLS: {
      char lenbuf[20];

      byte_fill(lenbuf, sizeof(lenbuf), ' ');
      lenbuf[fmt_ulong(lenbuf, pl->count)] = '\n';


      buffer_flush(b); 

      lseek(b->fd, pl->num_items_pos, 0);

      buffer_put(b, lenbuf, sizeof(lenbuf));
      buffer_flush(b);

      lseek(b->fd, 0, SEEK_END);

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

