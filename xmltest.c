#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/str.h"

#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
static buffer infile;
static buffer b;
void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, (int (*)()) & iscntrl);
  buffer_putsa(b, &esc);
}

const char* node_types[] = {
    "(null)", "XML_DOCUMENT", "XML_ELEMENT", "XML_TEXT",
};

static int depth = 0;

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  xmlnode* n;
  if(id != XML_ELEMENT) return 1;
  if(reader->closing) --depth;
  buffer_putm_4(buffer_1, node_types[id], " \"", name ? name->s : "", "\"");
  if(value) buffer_putm_2(buffer_1, ", value=", value ? value->s : "");
  buffer_puts(buffer_1, ", depth=");
  buffer_putlong(buffer_1, depth);
  buffer_puts(buffer_1, ", closing=");
  buffer_putlong(buffer_1, reader->closing);
  buffer_puts(buffer_1, ", self_closing=");
  buffer_putlong(buffer_1, reader->self_closing);
  buffer_putnlflush(buffer_1);
  if(!reader->closing && !reader->self_closing) ++depth;
  return 1;
}

int

main(int argc, char* argv[1]) {
  xmlreader r;
  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");
  xml_reader_init(&r, &infile);
  xml_read_callback(&r, xml_read_function);
  buffer_close(&b);
}
