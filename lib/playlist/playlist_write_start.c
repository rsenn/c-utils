#define _LARGEFILE64_SOURCE
#include "../windoze.h"
#include "../io_internal.h"
#include "../playlist.h"
#include "../byte.h"

#include <stdio.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
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

    pl->num_items_pos = io_seek(b->fd, 0, SEEK_CUR);

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

