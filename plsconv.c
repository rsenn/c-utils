#include "lib/windoze.h"
#include "lib/unix.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/open.h"
#include "lib/playlist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/fmt.h"
#include "lib/scan.h"
#include "lib/hmap.h"
#include "lib/xml.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>

/*extern ssize_t write();
 */
playlist pls1, pls2;
static buffer outfile;
static char outbuf[1024];

static buffer inbuf;

static const char*
mybasename(char* name) {
  size_t n = str_rchr(name, '/');
  if(name[n] != '\0')
    return &name[n + 1];
  return name;
}

static void
playlist_process(playlist* pl, stralloc* title, stralloc* location, uint32 length) {
  playlist_entry e;
  (void)pl;
  /*  buffer_putm_3(buffer_2, "Title: ", title->s, "\n");
    buffer_putm_3(buffer_2, "Location: ", location->s, "\n");
    buffer_puts(buffer_2, "Duration: ");
    buffer_putulong(buffer_2, length);
    buffer_putnlflush(buffer_2);

  */
  byte_zero(&e, sizeof(e));
  stralloc_copy(&e.title, title);
  stralloc_copy(&e.path, location);
  e.length = length;
  playlist_write_entry(&outfile, &pls1, &e);
}

void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       argv0,
                       " [-f type] [-t type] [-o output] [input or stdin]\n\n",
                       "  -f type  Input type\n"
                       "  -t type  Output type\n"
                       "\n"
                       "Supported types are: m3u pls xspf\n",
                       NULL);
  buffer_flush(buffer_1);
  exit(0);
}

int
main(int argc, char* argv[]) {

  int opt;

  const char *out_file = NULL, *in_type = NULL;
  playlist_type_fn* playlist_fn;
  /*playlist_type intype = M3U*/
  playlist_type outtype = XSPF;

  int out_fd = STDOUT_FILENO;

  while((opt = getopt(argc, argv, "o:f:t:h")) != -1) {
    switch(opt) {
      case 'o': out_file = optarg; break;
      case 'f': in_type = optarg; break;
      case 't':
        if(!str_diff(optarg, "pls"))
          outtype = PLS;
        else if(!str_diff(optarg, "m3u"))
          outtype = M3U;
        else if(!str_diff(optarg, "xspf"))
          outtype = XSPF;
        break;
      case 'h': usage(str_basename(argv[0])); exit(EXIT_SUCCESS);
      default: /* '?' */ buffer_putm_3(buffer_2, "Usage: ", argv[0], "[-t TYPE] [file]\n"); exit(EXIT_FAILURE);
    }
  }

  {
    const char* in_file = argc > optind ? argv[optind] : "-";
    int in_fd;

    if(!str_diff(in_file, "-")) {
      in_fd = STDIN_FILENO;
    } else if(in_file == NULL || (in_fd = open_read(in_file)) == -1) {
      buffer_putm_3(buffer_2, "No such file: ", in_file ? in_file : "(null)", "\n");
      buffer_flush(buffer_2);
      exit(1);
    }

    if(in_type == NULL) {
      size_t i = str_rchr(in_file, '.');
      in_type = &in_file[i];
      if(*in_type == '.')
        ++in_type;
    }

    if(!str_diff(in_type, ".pls"))
      playlist_fn = &playlist_pls;
    else if(!str_diff(in_type, "xspf"))
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
        buffer_putm_3(buffer_2, "Error writing: ", out_file ? out_file : "(null)", "\n");
        buffer_flush(buffer_2);
        exit(1);
      }
    }

    // buffer_init(&outfile, write, open_trunc("playlist.out"), outbuf, sizeof(outbuf));
    buffer_init(&outfile, (buffer_op_ptr)&write, out_fd, outbuf, sizeof(outbuf));

    playlist_write_start(&outfile, &pls1);

    // buffer_mmapread(&inbuf, "/home/roman/nectarine.m3u");

    buffer_read_fd(&inbuf, in_fd);

    playlist_fn(&pls2, &inbuf);

    pls2.callback = playlist_process;
    playlist_read(&pls2);
    buffer_close(&inbuf);

    playlist_write_finish(&outfile, &pls1);
    buffer_flush(&outfile);
    buffer_close(&outfile);
  }
  return 0;
}
