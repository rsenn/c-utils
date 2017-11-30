#include "playlist.h"

static void
playlist_process(playlist* pl, stralloc* title, stralloc* location, uint32 length) {
  buffer_putm(buffer_2, "Title: ", title->s, "\n", NULL);
  buffer_putm(buffer_2, "Location: ", location->s, "\n", NULL);
  buffer_puts(buffer_2, "Duration: ");
  buffer_putulong(buffer_2, length);
  buffer_putnlflush(buffer_2);
}

int main() {

  buffer inbuf;

  playlist pls;
  playlist_init(&pls);

  buffer_mmapread(&inbuf, "/home/roman/Dokumente/vlc.xspf");

  playlist_xspf(&pls, &inbuf);

  pls.callback = playlist_process;

  playlist_read(&pls);

}
