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

  playlist pls1, pls2;
  playlist_init(&pls1);
  playlist_init(&pls2);
  /*
    buffer_mmapread(&inbuf, "/home/roman/Dokumente/vlc.xspf");
    playlist_xspf(&pls1, &inbuf);
    pls1.callback = playlist_process;
    playlist_read(&pls1);
    buffer_close(&inbuf);
  */
  buffer_mmapread(&inbuf, "/home/roman/Unsorted Files/ARTE.m3u");
  playlist_m3u(&pls2, &inbuf);
  pls2.callback = playlist_process;
  playlist_read(&pls2);
  buffer_close(&inbuf);

}
