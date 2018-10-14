#include "../buffer.h"
#include "../playlist.h"
#include "../scan.h"
#include "../str.h"
#include <ctype.h>

static int
m3u_reader(playlist* pl) {
int ret;
  static playlist_entry entry;
  buffer* inbuf = pl->ptr;
  stralloc line;
  stralloc_init(&line);
  if((ret = buffer_getline_sa(inbuf, &line))) {
    while(line.len > 1 &&
          (line.s[line.len - 1] == '\r' || line.s[line.len - 1] == '\n'))
      line.len--;
    stralloc_0(&line);
    line.len -= 1;
    if(!str_diffn(line.s, "#EXTM3U", 7)) {
    } else if(!str_diffn(line.s, "#EXTINF", 7)) {
      size_t len;
      size_t index = 8;
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
playlist_m3u(playlist* pls, buffer* b) {
  pls->ptr = b;
  pls->type = M3U;
  pls->reader = (int (*)(playlist*))m3u_reader;
}
