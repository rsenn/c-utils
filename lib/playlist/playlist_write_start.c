#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */
#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#include "../io_internal.h"
#else
#include <unistd.h>
#endif

#include <stdio.h>


#include "../playlist.h"

#if !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__dietlibc__) && !defined(__APPLE__)
#define lseek lseek64
#endif

int
playlist_write_start(buffer* b, playlist* pl) {
  pl->count = 0;
  switch(pl->type) {
  case M3U: {
    buffer_puts(b, "#EXTM3U\n");
    break;
  }
  case PLS: {
    buffer_puts(b, "[playlist]\n");
    buffer_putsflush(b, "NumberOfEntries=");

    pl->num_items_pos = LSEEK(b->fd, 0, SEEK_CUR);

    buffer_puts(b, "XXXXXXXXXXXXXXXXXXXX\n");
    break;
  }
  case XSPF: {
    buffer_puts(b, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<playlist xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\" version=\"1\">\n  <title>Wiedergabeliste</title>\n  <trackList>\n");
    break;
  }
  case UNKNOWN:
  default: {
    return 0;
  }
  }
  return 1;
}

