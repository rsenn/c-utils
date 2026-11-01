#include "../buffer.h"
#include "../alloc.h"
#include "../byte.h"
#include "../playlist_internal.h"
#include "../scan.h"
#include "../str.h"
#include <ctype.h>

/* the entry with track number n (1 based), made if it is not there yet */
static playlist_entry*
pls_entry(struct playlist_state* st, size_t n) {
  if(n == 0 || n > 100000)
    return NULL;
  if(n > st->listlen) {
    playlist_entry* l = (playlist_entry*)alloc_zero(n * sizeof(playlist_entry));

    if(!l)
      return NULL;
    if(st->listlen)
      byte_copy(l, st->listlen * sizeof(playlist_entry), st->list);
    if(st->list)
      alloc_free(st->list);
    st->list = l;
    st->listlen = n;
  }
  return &st->list[n - 1];
}

/* Reads FileN=, TitleN= and LengthN= lines; the entries go to the callback in
 * track order when the input is at its end (the lines of one track may come in
 * any order and apart). */
static int
pls_reader(playlist* pl) {
  struct playlist_state* st = pl->ptr;
  stralloc line;
  int ret;

  stralloc_init(&line);
  if((ret = buffer_getline_sa(st->in, &line)) > 0) {
    size_t eq, key;

    while(line.len > 0 && (line.s[line.len - 1] == '\r' || line.s[line.len - 1] == '\n'))
      line.len--;
    stralloc_nul(&line);

    if(line.s[0] != '[' && (eq = str_chr(line.s, '=')) > 0 && line.s[eq]) {
      unsigned long no = 0;
      playlist_entry* e;

      key = eq;
      while(key > 0 && isdigit((unsigned char)line.s[key - 1]))
        --key;
      if(key < eq)
        scan_ulong(&line.s[key], &no);
      if(no && (e = pls_entry(st, no))) {
        if(key == 4 && !str_diffn(line.s, "File", 4))
          stralloc_copys(&e->path, &line.s[eq + 1]);
        else if(key == 5 && !str_diffn(line.s, "Title", 5))
          stralloc_copys(&e->title, &line.s[eq + 1]);
        else if(key == 6 && !str_diffn(line.s, "Length", 6)) {
          unsigned long len = 0;

          scan_ulong(&line.s[eq + 1], &len);
          e->length = (uint32)len;
        }
      }
    }
    stralloc_free(&line);
    return 1;
  }
  stralloc_free(&line);

  if(!st->flushed) { /* end of input: hand over the entries, once */
    size_t i;

    st->flushed = 1;
    for(i = 0; i < st->listlen; ++i) {
      if(st->list[i].path.len == 0)
        continue;
      ++pl->count;
      if(pl->callback)
        pl->callback(pl, &st->list[i].title, &st->list[i].path, st->list[i].length);
    }
  }
  return 0;
}

void
playlist_pls(playlist* pls, buffer* b) {
  pls->type = PLS;
  if(playlist_state_new(pls, b)) {
    pls->type = PLS;
    pls->reader = pls_reader;
  }
}
