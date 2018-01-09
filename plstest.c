#include "playlist.h"
#include "open.h"
#include "buffer.h"
#include "byte.h"
#include <unistd.h>

  playlist pls1, pls2;
  static buffer outfile;
  static char outbuf[1024];
  
static void
playlist_process(playlist* pl, stralloc* title, stralloc* location, uint32 length) {
  buffer_putm(buffer_2, "Title: ", title->s, "\n", NULL);
  buffer_putm(buffer_2, "Location: ", location->s, "\n", NULL);
  buffer_puts(buffer_2, "Duration: ");
  buffer_putulong(buffer_2, length);
  buffer_putnlflush(buffer_2);
  
  
  playlist_entry e; 
  
byte_zero(&e, sizeof(e));
stralloc_copy(&e.title, title);
stralloc_copy(&e.path, location);
e.length = length;
 playlist_write_entry(&outfile, &pls1, &e); 
}

int main() {

  buffer inbuf;


  playlist_init(&pls1);
  playlist_init(&pls2);
  /*
    buffer_mmapread(&inbuf, "/home/roman/Dokumente/vlc.xspf");
    playlist_xspf(&pls1, &inbuf);
    pls1.callback = playlist_process;
    playlist_read(&pls1);
    buffer_close(&inbuf);
  */
  
  
  playlist_settype(&pls1, PLS);

buffer_init(&outfile, write, open_trunc("playlist.out"), outbuf, sizeof(outbuf));
  
  playlist_write_start(&outfile, &pls1);
  
  buffer_mmapread(&inbuf, "/home/roman/startrek.m3u");
  playlist_m3u(&pls2, &inbuf);
  pls2.callback = playlist_process;
  playlist_read(&pls2);
  buffer_close(&inbuf);

buffer_flush(&outfile);
}
