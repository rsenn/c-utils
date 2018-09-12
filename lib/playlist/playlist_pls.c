#include "../buffer.h"
#include "../playlist.h"
#include "../scan.h"
#include "../str.h"
#include <ctype.h>

static int
pls_reader(playlist* pl) {
  int ret;
    static playlist_entry entry;
  buffer* inbuf = pl->ptr;
  stralloc line;
  stralloc_init(&line);
  if(( ret = buffer_getline_sa(inbuf, &line))) {
    uint32 index2, index;
    index2 = index = 0;
    while(line.len > 1 &&
          (line.s[line.len - 1] == '\r' || line.s[line.len - 1] == '\n'))
      line.len--;
    stralloc_0(&line);
    if(!str_diffn(&line.s[index], "Number", 6)) {
    } else if(line.s[index] == '[') {
    } else if((index2 = str_chr(&line.s[index], '=')) > 0) {
      unsigned long trackno = 0;
      index = index2;
      index2++;
      do { index--; } while(isdigit(line.s[index]) && index > 0);
      scan_ulong(&line.s[index], &trackno);
      if(!str_diffn(&line.s[index], "File", 4)) {
        stralloc_copys(&entry.path, &line.s[index2]);
        stralloc_0(&entry.path);
      } else if(!str_diffn(&line.s[index], "Title", 5)) {
        stralloc_copys(&entry.title, &line.s[index2]);
        stralloc_0(&entry.title);
      } else if(!str_diffn(&line.s[index], "Length", 6)) {
        unsigned long len;
        scan_ulong(&line.s[index2], &len);
        entry.length = len;
      }
      /*
      uint32 index = 8;
      index += scan_ulong(&line.s[index], &len);
      entry.length = len;
      index++;
      stralloc_copys(&entry.title, &line.s[index]);
      stralloc_0(&entry.title);
      */
    } else {
      /*
      stralloc_copy(&entry.path, &line);
      stralloc_0(&entry.path);
      if(pl->callback) {
      pl->callback(pl, &entry.title, &entry.path, entry.length);
      }*/
    }
  }
  return ret;
}

void
playlist_pls(playlist* pls, buffer* b) {
  pls->ptr = b;
  pls->type = PLS;
  pls->reader = (int (*)(playlist*))pls_reader;
}
