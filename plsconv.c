#include "playlist.h"
#include "open.h"
#include "buffer.h"
#include "byte.h"
#include "str.h"
#include <unistd.h>

playlist pls1, pls2;
static buffer outfile;
static char outbuf[1024];

static  buffer inbuf;


static void
playlist_process(playlist* pl, stralloc* title, stralloc* location, uint32 length) {
/*  buffer_putm(buffer_2, "Title: ", title->s, "\n", NULL);
  buffer_putm(buffer_2, "Location: ", location->s, "\n", NULL);
  buffer_puts(buffer_2, "Duration: ");
  buffer_putulong(buffer_2, length);
  buffer_putnlflush(buffer_2);

*/

  playlist_entry e;

  byte_zero(&e, sizeof(e));
  stralloc_copy(&e.title, title);
  stralloc_copy(&e.path, location);
  e.length = length;
  playlist_write_entry(&outfile, &pls1, &e);
}

int main(int argc, char *argv[]) {

  int opt;

  const char *out_file = NULL, *in_type = NULL;
  playlist_type_fn* playlist_fn;
  playlist_type intype = M3U, outtype = XSPF;
  int out_fd = STDOUT_FILENO;

  while((opt = getopt(argc, argv, "o:f:t:")) != -1) {
    switch(opt) {
    case 'o':
      out_file = optarg;
      break;
    case 'f':
      in_type = optarg;
      break;
    case 't':
      if(!str_diff(optarg, "pls")) outtype = PLS;
      else if(!str_diff(optarg, "m3u")) outtype = M3U;
      else if(!str_diff(optarg, "xspf"))  outtype = XSPF;
      break;
    default: /* '?' */
      buffer_putm(buffer_2, "Usage: ", argv[0], "[-t TYPE] [file]\n");
      exit(EXIT_FAILURE);
    }
  }

  const char* in_file = argv[optind];
  int in_fd; 

  if(in_file == NULL || (in_fd = open_read(in_file)) == -1) {
    buffer_putm(buffer_2, "No such file: ", in_file ? in_file : "(null)", "\n");
    buffer_flush(buffer_2);
    exit(1);
  }

  if(in_type == NULL) {
    size_t i = str_rchr(in_file, '.');
    in_type = &in_file[i];
    if(*in_type == '.') ++in_type;
  }
  
  /*if(!str_diff(&in_file[i], ".pls"))
    playlist_fn = &playlist_pls;
  else */if(!str_diff(in_type, "xspf"))
    playlist_fn = &playlist_xspf /*, intype = XSPF*/;
  else
    playlist_fn = &playlist_m3u;

  playlist_init(&pls1);
  playlist_init(&pls2);
  /*
    buffer_mmapread(&inbuf, "/home/roman/Dokumente/vlc.xspf");
    playlist_xspf(&pls1, &inbuf);
    pls1.callback = playlist_process;
    playlist_read(&pls1);
    buffer_close(&inbuf);
  */

  playlist_settype(&pls1, outtype);


  if(out_file != NULL) {
     out_fd = open_trunc(out_file);
     if(out_fd == -1) {
      buffer_putm(buffer_2, "Error writing: ", out_file ? out_file : "(null)", "\n");
      buffer_flush(buffer_2);
      exit(1);
     }
  }

 //buffer_init(&outfile, write, open_trunc("playlist.out"), outbuf, sizeof(outbuf));
  buffer_init(&outfile, write, out_fd, outbuf, sizeof(outbuf));
// outfile.deinit  = &buffer_close;

  playlist_write_start(&outfile, &pls1);

  //buffer_mmapread(&inbuf, "/home/roman/nectarine.m3u");
  buffer_mmapread_fd(&inbuf, in_fd);

  playlist_fn(&pls2, &inbuf);

  pls2.callback = playlist_process;
  playlist_read(&pls2);
  buffer_close(&inbuf);

  playlist_write_finish(&outfile, &pls1);
  buffer_flush(&outfile);
  buffer_close(&outfile);
}
