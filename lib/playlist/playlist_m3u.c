#include "../buffer.h"
#include "../playlist_internal.h"
#include "../scan.h"
#include "../str.h"

/* One line per call: #EXTM3U and other comments are skipped, #EXTINF:len,title
 * sets title and length of the entry that the next path line completes. */
static int
m3u_reader(playlist* pl) {
  struct playlist_state* st = pl->ptr;
  stralloc line;
  int ret;

  stralloc_init(&line);
  if((ret = buffer_getline_sa(st->in, &line)) > 0) {
    while(line.len > 0 && (line.s[line.len - 1] == '\r' || line.s[line.len - 1] == '\n'))
      line.len--;
    stralloc_nul(&line);

    if(line.len == 0) {
      /* blank line */
    } else if(!str_diffn(line.s, "#EXTINF:", 8)) {
      unsigned long len = 0;
      size_t index = 8;
      int neg = line.s[index] == '-';

      if(neg)
        ++index;
      index += scan_ulong(&line.s[index], &len);
      st->entry.length = neg ? 0 : (uint32)len; /* -1 means unknown */
      if(line.s[index] == ',')
        ++index;
      stralloc_copys(&st->entry.title, &line.s[index]);
    } else if(line.s[0] == '#') {
      /* #EXTM3U, #EXTVLCOPT, any other comment */
    } else {
      stralloc_copyb(&st->entry.path, line.s, line.len);
      ++pl->count;
      if(pl->callback)
        pl->callback(pl, &st->entry.title, &st->entry.path, st->entry.length);
      playlist_entry_clear(&st->entry);
    }
  }
  stralloc_free(&line);
  return ret > 0 ? 1 : 0;
}

void
playlist_m3u(playlist* pls, buffer* b) {
  pls->type = M3U;
  if(playlist_state_new(pls, b)) {
    pls->type = M3U;
    pls->reader = m3u_reader;
  }
}
