#include "../windoze.h"
#include "../io.h"
#include "../playlist.h"
#include "../byte.h"
#include "../fmt.h"

#include <sys/types.h>

#if WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

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

      io_seek(b->fd, pl->num_items_pos, 0);

      buffer_put(b, lenbuf, sizeof(lenbuf));
      buffer_flush(b);

      io_seek(b->fd, 0, SEEK_END);

      break;
    }
    case XSPF: {
      buffer_puts(b, "  </trackList>\n</playlist>\n");
      break;
    }
    case UNKNOWN:
    default: {
               return 0;
    }
  }
  buffer_flush(b);
  return 1;
}

