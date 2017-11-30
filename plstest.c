#include "playlist.h"

int main() {

  buffer inbuf;

  playlist pls;
  playlist_init(&pls);

  buffer_mmapread(&inbuf, "/home/roman/Dokumente/videos.xspf");

  playlist_xspf(&pls, &inbuf);
}
