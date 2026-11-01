#include "../buffer.h"
#include "../byte.h"
#include "../playlist_internal.h"
#include "../scan.h"
#include "../xml.h"
#include "../str.h"

static int
xspf_xml_callback(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  playlist* pl = reader->data;
  struct playlist_state* st = pl->ptr;
  playlist_entry* e = &st->entry;

  (void)attrs;
  if(id == XML_ELEMENT) {
    stralloc_copy(&st->tag, name);
    stralloc_nul(&st->tag);
    if(reader->closing && str_equal(st->tag.s, "track")) {
      ++pl->count;
      if(pl->callback)
        pl->callback(pl, &e->title, &e->path, e->length);
      playlist_entry_clear(e);
    }
  } else if(id == XML_TEXT && st->tag.s) {
    if(str_equal(st->tag.s, "location"))
      stralloc_copy(&e->path, value);
    else if(str_equal(st->tag.s, "title"))
      stralloc_copy(&e->title, value);
    else if(str_equal(st->tag.s, "duration")) {
      unsigned long ms = 0;

      stralloc_nul(value);
      scan_ulong(value->s, &ms);
      e->length = (uint32)ms;
    }
  }
  return 1;
}

/* the whole document is read by one call */
static int
xspf_reader(playlist* pl) {
  struct playlist_state* st = pl->ptr;

  if(st->flushed)
    return 0;
  st->flushed = 1;
  xml_read_callback(&st->reader, &xspf_xml_callback);
  return 0;
}

void
playlist_xspf(playlist* pls, buffer* b) {
  struct playlist_state* st;

  pls->type = XSPF;
  if(!(st = playlist_state_new(pls, b)))
    return;
  pls->type = XSPF;
  xml_reader(&st->reader, b);
  st->reader.data = pls; /* the state lives on the heap now; this used to be a stack variable */
  pls->reader = xspf_reader;
}
