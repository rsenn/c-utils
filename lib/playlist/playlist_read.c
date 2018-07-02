#include "../playlist.h"
#include "../byte.h"

int
playlist_read(playlist* pl) {
  int ret;
  int i = 0;

  while((ret = pl->reader(pl)) > 0) {
    i += ret;
  }

  return i;
}

