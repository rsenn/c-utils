#include "../buffer.h"
#include "../byte.h"
#include "../playlist.h"
#include "../scan.h"
#include "../xml.h"

#include <ctype.h>
#include <stdlib.h>

typedef struct {
  stralloc tag;
  stralloc title, location;
  uint32 length;
  playlist* pl;
} track;

int
buffer_close_return_int(buffer* b) {
  buffer_close(b);
  return 0;
}

static void
xspf_process(playlist* pl, track* t, const char* tag, stralloc* value) {
  if(str_equal(t->tag.s, "location")) {
    stralloc_copy(&t->location, value);
  } else if(str_equal(t->tag.s, "title")) {
    stralloc_copy(&t->title, value);
  } else if(str_equal(t->tag.s, "duration")) {
    scan_uint(value->s, &t->length);
  }
}

static int
xspf_xml_callback(xmlreader* reader,
                  xmlnodeid id,
                  stralloc* name,
                  stralloc* value,
                  HMAP_DB** attrs) {
  track* t = reader->data;
  playlist* pl = t->pl;

  if(id == XML_ELEMENT) {
    stralloc_copy(&t->tag, name);
    stralloc_nul(&t->tag);

    if(reader->closing && str_equal(t->tag.s, "track")) {
      pl->callback(pl, &t->title, &t->location, t->length);
    }

  } else if(id == XML_TEXT) {
    xspf_process(pl, t, t->tag.s, value);
  }

  return 1;
}

static int
xspf_reader(playlist* pl) {
  xml_read_callback(pl->ptr, &xspf_xml_callback);
  return 0;
}

void
playlist_xspf(playlist* pls, buffer* b) {
   track* t;
    xmlreader rd;
  xml_reader_init(&rd, b);
t = malloc(sizeof(track));
  if(t == NULL) return;

  byte_zero(t, sizeof(track));
  t->pl = pls;
  rd.data = t;

  pls->ptr = &rd;
  pls->type = XSPF;

  pls->reader = (int (*)(playlist*))xspf_reader;
}
