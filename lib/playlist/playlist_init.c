#include "../playlist.h"
#include "../byte.h"

void
playlist_init(playlist* pl) {
  byte_zero(pl, sizeof(playlist));
}

