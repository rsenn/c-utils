
#include <libxml/xmlreader.h>
#include <ctype.h>
#include "../playlist.h"
#include "../buffer.h"
#include "../str.h"
#include "../scan.h"

static int
m3u_reader(playlist* pl) {
  static playlist_entry entry;
  buffer* inbuf = pl->ptr;

  stralloc line;
  stralloc_init(&line);

  int ret = buffer_getline_sa(inbuf, &line);

  if(ret) {

    while(line.len > 1 && (line.s[line.len - 1] == '\r' || line.s[line.len - 1] == '\n'))
      line.len--;
    stralloc_0(&line);
    line.len -= 1;


    if(!str_diffn(line.s, "#EXTM3U", 7)) {

    } else if(!str_diffn(line.s, "#EXTINF", 7)) {
      unsigned long len;
      uint32 index = 8;

      index += scan_ulong(&line.s[index], &len);
      entry.length = len;
      index++;
      stralloc_copys(&entry.title, &line.s[index]);
      stralloc_0(&entry.title);
      entry.title.len -= 1;

    } else if(!str_diffn(line.s, "#EXTVLCOPT", 10)) {

    } else {
      stralloc_copy(&entry.path, &line);
      stralloc_0(&entry.path);
      entry.path.len -= 1;

      if(pl->callback) {

        pl->callback(pl, &entry.title, &entry.path, entry.length);
      }
    }
  }

  return ret;
}

void
playlist_m3u(playlist* pls, buffer *b) {
  pls->ptr = b;
  pls->type = M3U;
  pls->reader = (int(*)(playlist*))m3u_reader;
}
