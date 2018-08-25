#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static buffer infile, b;
static int depth = 0, prev_closing = 0;
static stralloc prev_element;


void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, (int (*)()) & iscntrl);
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
  xmlnode* n;
  int closing = reader->closing || reader->self_closing;
  if(id != XML_ELEMENT) return 1;
  if(reader->closing) --depth;

  switch(id) {
    case XML_ELEMENT: {
        if(!(reader->closing && stralloc_equal(&prev_element, name))) {
            buffer_puts(buffer_1, "\n");
            buffer_putnspace(buffer_1, depth * 2);
          }


        buffer_putm(buffer_1, "<", reader->closing ? "/" : "", name->s);


        if(attrs && hmap_size(*attrs)) {
            xml_print_attributes(*attrs, buffer_1, " ", "=", "\"");

          }

        buffer_puts(buffer_1, reader->self_closing ? "/>" : ">");

          stralloc_copy(&prev_element, name);
        break;
      }
    }
//  buffer_putm(buffer_1, node_types[id], " \"", name ? name->s : "", "\"");
//  if(value) buffer_putm(buffer_1, ", value=", value ? value->s : "");
//  buffer_puts(buffer_1, ", depth=");
//  buffer_putlong(buffer_1, depth);
//  buffer_puts(buffer_1, ", closing=");
//  buffer_putlong(buffer_1, reader->closing);
//  buffer_puts(buffer_1, ", self_closing=");
//  buffer_putlong(buffer_1, reader->self_closing);
//  buffer_putnlflush(buffer_1);
  if(!reader->closing && !reader->self_closing) ++depth;
  prev_closing = closing;
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
