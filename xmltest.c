#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static buffer infile;
static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  byte_fmt_pred(str, str_len(str), &esc, fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

const char* node_types[] = {
  "(null)",
  "XML_DOCUMENT",
  "XML_ELEMENT",
  "XML_TEXT",
};

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {


  if(id == XML_ATTRIBUTE) return 1;

  buffer_putm(buffer_1, "node type=", node_types[id], ", name=", name ? name->s : "", ", value=", value ? value->s : "");

  buffer_puts(buffer_1, ", closing=");
  buffer_putlong(buffer_1, reader->closing);

  buffer_puts(buffer_1, ", self_closing=");
  buffer_putlong(buffer_1, reader->self_closing);

  buffer_putnlflush(buffer_1);

  return 1;
}

int
main(int argc, char* argv[1]) {

  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");

  xmlreader r;
  xml_reader_init(&r, &infile);
  xml_read_callback(&r, xml_read_function);

  buffer_close(&b);
}
