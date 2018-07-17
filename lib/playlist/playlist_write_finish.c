#include "../io_internal.h"
#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */
#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#else
#include <unistd.h>
#endif

#include <stdio.h>

#include "../playlist.h"
#include "../byte.h"
#include "../fmt.h"

#if !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__dietlibc__) && !defined(__APPLE__)
#define lseek lseek64
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

      LSEEK(b->fd, pl->num_items_pos, 0);

      buffer_put(b, lenbuf, sizeof(lenbuf));
      buffer_flush(b);

      LSEEK(b->fd, 0, SEEK_END);

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

